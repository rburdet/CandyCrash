#include "server.partida.h"
#include "common.events.h"
#include "common.logger.h"
#include "server.listador.h"
#include <sstream>
#include "common.user_manager.h"

using std::string;
using Json::Value;
using Json::StaticString;
using std::stringstream;

Partida::Partida(ServerInterface* server, int nivel, string& nombre) : server(server), nivel(nivel), nombre(nombre), estado(PARTIDA_ABIERTA), tablero(NULL) {
	Json::Value val;
	Listador::getMapa(this->nombre, val);
	Json::Value::Members keys = val.getMemberNames();

	if(keys.size()){
		std::string nMapa = keys[0];
		this->mapa = val[nMapa];
	}
	this->maxUsuarios = this->mapa["max_jugadores"].asInt();
	this->puntosMax = this->mapa["puntaje_para_ganar"].asInt();

}

Partida::~Partida() {
	this->server->removePartida(this);
	this->usuariosLock.lock();
	for(unsigned int i=0; i < this->usuarios.size(); i++){
		// Decirles que se bajo la partida

	}
	this->usuariosLock.unlock();
	this->tableroLock.lock();
	delete this->tablero;
	this->tablero = NULL;
	this->tableroLock.unlock();
}


void Partida::addUsuario(ThreadSocket* u, Json::Value& user){
	Value retMsj;
	retMsj["event"] = EVENT_GAME_USER_ADD;
	retMsj["msj"] = "Se conecto "+user["user"].asString();
	retMsj["user"] = user;
	retMsj["code"] = 0;

	this->broadcastMsj(retMsj);

	this->usuariosLock.lock();
	this->usuarios.push_back(u);
	user["puntos"] = 0;
	stringstream ss;
	ss << (long) u;
	user["id"] = ss.str();
	this->usuarios_data.push_back(user);
	this->usuariosLock.unlock();
}

void Partida::rmUsuario(ThreadSocket* u){
	Value retMsj;
	this->usuariosLock.lock();

	for(unsigned int i=0; i < usuarios.size(); i++){
		if((void*) u == (void*) (this->usuarios[i])){
			retMsj["user"] = this->usuarios_data[i];
			break;
		}
	}
	retMsj["event"] = EVENT_GAME_USER_RM;
	retMsj["msj"] = "Usuario desconectado "+retMsj["user"]["user"].asString();
	retMsj["code"] = 0;

	for(unsigned int i=0; i < usuarios.size(); i++){
		if((void*) u == (void*) (this->usuarios[i])){
			this->usuarios.erase(this->usuarios.begin() + i);
			this->usuarios_data.erase(this->usuarios_data.begin() + i);
		}else{
			if(this->usuarios[i]->write(retMsj)){
				Logger::log("[Partida] Error broadcasteando mensjae de nuevo usuario");
			}
		}
	}
	this->usuariosLock.unlock();

	if(this->usuarios.size() == 0)
		delete this;
}

int Partida::getNivel(){
	return this->nivel;
}

int Partida::getUsuarios(){
	return this->usuarios.size();
}

PartidaEstado Partida::getEstado(){
	return this->estado;
}

int Partida::mensaje(Json::Value& data, ThreadSocket* u){
	StaticString def("");
	CommonEvents event = EVENT_NONE;
	int code;

	if(data.get("ev-game", def).isNumeric())
		event = (CommonEvents) data.get("ev-game", def).asInt();

	if(data.get("code", def).isNumeric())
		code = (CommonEvents) data.get("code", def).asInt();

	switch(event){
		case EVENT_GAME_CHAT:{
			string str = data["line"].asString();
			Json::Value send;
			send["event"] = EVENT_GAME_CHAT;
			send["line"] = str;
			this->broadcastMsj(send);
			break;
		}

		case EVENT_GAME_INFO:{
			this->usuariosLock.lock();
			Json::Value send;
			send["event"] = EVENT_GAME_INFO;
			send["users"];
			for(int i=0; i < this->usuarios_data.size() ; i++){
				send["users"][i] = this->usuarios_data[i];
			}
			this->usuariosLock.unlock();
			u->write(send);
			break;
		}
		case EVENT_GAME_START:{
			Json::Value send;
			if(u != usuarios[0]){ // meow meow meow?
				send["event"] = EVENT_GAME_MSG;
				send["msg"] = "No sos el administrador";
				send["code"] = 0;
				u->write(send);
				break;
			}
			this->tableroLock.lock();
			if(this->estado != PARTIDA_ABIERTA){
				this->msjError(u, "La partida no esta abierta");
				this->tableroLock.unlock();
				break;
			}
			this->estado = PARTIDA_JUGANDO;
			Logger::log("Debo empezar partida!");
			send["event"] = EVENT_GAME_START;
			this->tablero = new Tablero(nombre);
			send["tablero"] = this->tablero->getTablero();
			this->tableroLock.unlock();
			this->broadcastMsj(send);
			break;
		}

		case EVENT_GAME_MOV:{
			std::cout << "Me llego un evento de movimiento x:" << data["x"].asInt() << " y:" << data["y"].asInt() << std::endl;
			// Me llegan los movimientos, se los tengo qe pasar al tablero
			this->tableroLock.lock();
			if(this->estado != PARTIDA_JUGANDO){
				this->msjError(u, "La partida no esta activa");
				this->tableroLock.unlock();
				break;
			}
			// TODO: controlar que vengan todos los datos en el mensaje
			Json::Value movimientos;
			int puntos = this->tablero->movimiento(data["x"].asInt(), data["y"].asInt(), (CaramelosMovimientos) data["mov"].asInt(), movimientos);
			if(puntos){ // Si el movimiento es de 0 puntos, significa qe fue un movimiento invalido, sino, es valido
				Logger::log("puntos");
				std::cout << "Movimientos " << movimientos << std::endl;
				Json::Value send;
				send["event"] = EVENT_GAME_MOV;
				send["code"] = 0;
				send["movs"] = movimientos;
				this->broadcastMsj(send);
				this->usuariosLock.lock();
				Json::Value user;
				// Le sumo los puntos al jugador
				for(unsigned int i=0; i < usuarios.size(); i++){
					if((void*) u == (void*) (this->usuarios[i])){
						user = this->usuarios_data[i];
						puntos += this->usuarios_data[i]["puntos"].asInt();
						this->usuarios_data[i]["puntos"] = puntos;
						break;
					}
				}
				this->usuariosLock.unlock();
				Json::Value end_send;
				end_send["event"] = EVENT_GAME_END;
				end_send["code"] = 0;

				// Me tengo que fijar si se termino el juego
				if(puntos >= puntosMax){ // El jugador supero el puntage maximo?
					Json::Value user_data;
					UserManager::get(user["user"].asString(), user_data);
					user_data["nivel"] = user_data["nivel"].asInt() +1;
					UserManager::set(user_data);
					this->estado = PARTIDA_TERMINADA;
					end_send["user"] = user;
					end_send["msg"] = "Gano "+user["user"].asString();

					this->broadcastMsj(end_send);
				}else if(!this->tablero->hayMovimientos()){ // No se pueden hacer mas movimientos?
					this->estado = PARTIDA_TERMINADA;
					end_send["code"] = 1;
					end_send["msg"] = "Nadie gano";
					this->broadcastMsj(end_send);
				}
			}
			this->tableroLock.unlock();
			break;
		}

		default:
			break;
	}

	return 0;
}

std::string Partida::getNombre(){
	return this->nombre;
}

int Partida::getMaxUsuarios(){
	return this->maxUsuarios;
}

void Partida::broadcastMsj(Json::Value& msj){
	this->usuariosLock.lock();
	for(unsigned int i=0; i < this->usuarios.size(); i++){
		if(this->usuarios[i]->write(msj))
			Logger::log("[Partida] Error broadcasteando mensaje.");
	}
	this->usuariosLock.unlock();
}

void Partida::msjError(ThreadSocket* u, char* msj){
	std::string str(msj);
	this->msjError(u, str);
}

void Partida::msjError(ThreadSocket* u, std::string& msj){
	Json::Value send;
	send["event"] = EVENT_GAME_MSG;
	send["msg"] = msj;
	send["code"] = 0;
	u->write(send);
}

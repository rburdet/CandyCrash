#include "server.partida.h"
#include "common.events.h"
#include "common.logger.h"
#include "server.listador.h"

using std::string;
using Json::Value;
Partida::Partida(ServerInterface* server, int nivel, string& nombre) : server(server), nivel(nivel), nombre(nombre), estado(PARTIDA_ABIERTA) {
	Json::Value val;
	Listador::getMapa(this->nombre, val);
	Json::Value::Members keys = val.getMemberNames();

	if(keys.size()){
		std::string nMapa = keys[0];
		this->mapa = val[nMapa];
	}
	this->maxUsuarios = this->mapa["max_jugadores"].asInt();

}

Partida::~Partida() {
	this->usuariosLock.lock();
	for(unsigned int i=0; i < this->usuarios.size(); i++){
		// Decirles que se bajo la partida

	}
	this->usuariosLock.unlock();
	this->server->removePartida(this);
}


void Partida::addUsuario(ThreadSocket* u, string& user){
	Value retMsj;
	retMsj["event"] = EVENT_GAME_MISC;
	retMsj["msj"] = "Se conecto "+user;
	retMsj["code"] = 0;

	this->usuariosLock.lock();
	// TODO: Dar msje de bievenida a la partida
	for(unsigned int j=0; j < this->usuarios.size(); j++){
		if(this->usuarios[j]->write(retMsj)){
			Logger::log("[Partida] Error broadcasteando mensjae de nuevo usuario");
		}
	}
	this->usuarios.push_back(u);
	this->usuariosLock.unlock();
}

void Partida::rmUsuario(ThreadSocket* u){
	Value retMsj;
	retMsj["event"] = EVENT_GAME_MISC;
	retMsj["msj"] = "Usuario desconectado (te debo el nombre)";
	retMsj["code"] = 0;

	this->usuariosLock.lock();
	for(unsigned int i=0; i < usuarios.size(); i++){
		if((void*) u == (void*) (this->usuarios[i])){
			this->usuarios.erase(this->usuarios.begin() + i);
		}else{
			if(this->usuarios[i]->write(retMsj)){
				Logger::log("[Partida] Error broadcasteando mensjae de nuevo usuario");
			}
		}
	}
	this->usuariosLock.unlock();
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

int Partida::mensaje(Json::Value& m){
	return 0;
}

std::string Partida::getNombre(){
	return this->nombre;
}

int Partida::getMaxUsuarios(){
	return this->maxUsuarios;
}

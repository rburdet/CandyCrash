#include "server.thread_usuario.h"

#include <jsoncpp/json/json.h>
#include <string>
#include <sstream>

#include "../common/common.logger.h"
#include "../common/common.user_manager.h"

#include "server.listador.h"

using Json::Value;
using Json::StaticString;
using std::string;
using std::stringstream;

ThreadUsuario::ThreadUsuario(ServerInterface* s, SocketIO* fd) : 
	ThreadSocket(fd), server(s), partida(NULL), user("") {}

ThreadUsuario::~ThreadUsuario(){
	Logger::log("["+this->myId+"] Cerrando thread");
	if(this->partida)
		this->partida->rmUsuario(this);
}


void* ThreadUsuario::subRun(){
	if(!this->welcome()){
		int ret_read;
		while(!(ret_read = this->read(false))){}

		if(ret_read == -1){
			Logger::log("["+this->myId+"] Escuchando evento cifrado");

			while(! this->read()){}
		}

		Logger::log("["+this->myId+"] Termino coneccion");
		this->server->removeClient(this);
	}

	return NULL;
}

int ThreadUsuario::welcome(){
	Value connect_msje;
	connect_msje["msj"] = "conectado";
	connect_msje["code"] = 0;

	//if(this->fd->write(connect_msje, this->pass)){
	if(this->write(connect_msje)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de bienvenida");
		return 1;
	}
	return 0;
}

int ThreadUsuario::eventNoFirmado(Value& data){
	StaticString def("");
	CommonEvents event = EVENT_NONE;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	switch(event){
		case EVENT_LOGIN: { // TODO: se podria pasar a una funcion
			Value connect_msje;
			int ret = 0;

			std::string tUser = data.get("user", def).asString();
			this->key = data.get("pass", def).asString();

			Logger::log("["+this->myId+"] Evento Login '"+tUser+"' '"+
					this->key+"'");
			Value userData;
			connect_msje["event"] = (int) EVENT_LOGIN;
			UserManager::get(tUser, userData);
			if(userData["user"] != tUser || userData["pass"] != this->key){
				Logger::log("["+this->myId+"] Error Login ");
				connect_msje["msj"] = "Usuario inexistente o password invalida";
				connect_msje["code"] = 1;
				ret = 0;
			}else{
				if(this->server->userConectado(tUser)){
					Logger::log("["+this->myId+"] Error Login: ya se encontraba conectado ");
					connect_msje["msj"] = "El usuario ya se encunetra conectado";
					connect_msje["code"] = 1;
					ret = 0;
				}else{
					Value ud;
					ud["user"] = userData["user"];
					ud["nivel"] = userData["nivel"];
					connect_msje["msj"] = "login correcto";
					connect_msje["code"] = 0;
					connect_msje["user"] = ud;
					this->user = tUser;
					ret = -1;
				}
			}

			if(this->write(connect_msje)){
				Logger::log("["+this->myId+"] Error escribiendo el mensaje de" 
						"confirmacion de login");
				return 1;
			}
			return ret;
			break;
		}

		case EVENT_NEW_USER:{ // TODO: se podria pasar a una funcion
			Value retMsj;
			int ret = 0;
			retMsj["event"] = EVENT_NEW_USER;

			std::string tUser = data.get("user", def).asString();
			this->key = data.get("pass", def).asString();
			Logger::log("["+this->myId+"] Evento new user '"+tUser+"' '"
					+this->key+"'");

			Value userData;
			UserManager::get(tUser, userData);
			if(userData.isNull() && tUser != string("") && 
					this->key != string("")){ // No existe usuario, se puede crear
				Value newUser;
				newUser["user"] = tUser;
				newUser["pass"] = this->key;
				newUser["nivel"] = 1;
				UserManager::set(newUser);

				retMsj["msj"] = "usuario creado correctamente";
				retMsj["code"] = 0;
				retMsj["user"] = tUser;
				retMsj["pass"] = this->key;
				ret = -1;
			}else{
				retMsj["msj"] = "error creando usuario";
				retMsj["code"] = 1;
				ret = -1;
			}

			if(this->write(retMsj)){
				Logger::log("["+this->myId+"] Error escribiendo el mensaje de "
						"confirmacion de login");
				return 1;
			}
			return ret;
			break;
		}

		default:
			Logger::log("["+this->myId+"] Evento desconocido");
			break;
	}

	return 0;
}

int ThreadUsuario::eventFirmado(Value& data){
	StaticString def("");
	CommonEvents event = EVENT_NONE;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	Value userData;
	UserManager::get(this->user, userData);

	switch(event){
		case EVENT_LIST_GAMES:{ // -> Listar partidas
			Logger::log("["+this->myId+"] Evento listar partidas");
			Value partidasList;
			this->server->listPartidas(userData["nivel"].asInt(), partidasList);
			Value retMsj;
			retMsj["event"] = EVENT_LIST_GAMES;
			retMsj["msj"] = "Ok";
			retMsj["code"] = 0;
			retMsj["partidas"] = partidasList;
			if(this->write(retMsj)){
				Logger::log("["+this->myId+"] Error escribiendo el"
						" mensaje de nueva partida");
				return 1;
			}
			return 0;
			break;
		}

		case EVENT_NEW_GAME:{ // -> Crear partida
			Logger::log("["+this->myId+"] Evento new game");
			return this->onNewGame(data, userData);
			break;
		}

		case EVENT_JOIN_GAME: // -> Unirse a partida
			return this->onJoinGame(data, userData);
			break;

		case EVENT_LEAVE_GAME: // -> dejar a partida
			return this->onLeaveGame(data, userData);
			break;

		case EVENT_GAME_MISC:
			Logger::log("["+this->myId+"] Evento game misc");
			if(this->partida && this->partida->mensaje(data, this)){
				this->partida = NULL;
			}
			return 0;
			break;

		case EVENT_GET_MAPS:
			Logger::log("Evento get maps");
			return this->onGetMaps(data, userData);
			break;

		default:
			Logger::log("["+this->myId+"] Evento desconocido");
			break;
	}

	return 0;
}

int ThreadUsuario::onLeaveGame(Json::Value& data, Json::Value& userData){
	this->partida->rmUsuario(this);
	this->partida = NULL;
	Value retMsj;
	retMsj["event"] = EVENT_LEAVE_GAME;
	retMsj["msj"] = "Dejaste la partida";
	retMsj["code"] = 0;
	if(this->write(retMsj)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de"
				" salida de partida");
		return 1;
	}
	return 0;
}

int ThreadUsuario::onJoinGame(Json::Value& data, Json::Value& userData){
	int ret = 0;
	Logger::log("["+this->myId+"] Evento Join game");
	Value retMsj;
	retMsj["event"] = EVENT_JOIN_GAME;
	retMsj["msj"] = "Error";
	retMsj["code"] = -1;
	if(this->partida){
		retMsj["msj"] = "Ya estas conectado a una partida";
		retMsj["code"] = 1;
	}else{
		string id = data["id"].asString();
		long id_n;
		stringstream ss(id);
		ss >> id_n;

		PartidaInterface* part = this->server->connectPartidas(id_n);

		if (part){
			if (part->getEstado() != PARTIDA_ABIERTA){
				retMsj["msj"] = "Partida no esta en estado abierta";
			}else if (part->getUsuarios() >= part->getMaxUsuarios()){
				retMsj["msj"] = "Partida llena";
			}else{
				Json::Value::Members keys = userData.getMemberNames();
				Json::Value yo;
				for(int i=0; i < keys.size(); i++){
					if(keys[i] != string("pass"))
						yo[keys[i]] = userData[keys[i]];
				}

				this->partida = part;
				part->addUsuario(this, yo);
				retMsj["msj"] = "Coneccion correcta";
				retMsj["code"] = 0;
			}
		}
	}

	if (this->write(retMsj)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de conexion"
				"de partida");
		return 1;
	}
	return ret;
}

int ThreadUsuario::onNewGame(Json::Value& data, Json::Value& userData){
	Value retMsj;
	int ret = 0;
	retMsj["event"] = EVENT_NEW_GAME;
	retMsj["msj"] = "Error";
	retMsj["code"] = 1;

	if (this->partida){
		retMsj["msj"] = "Ya estas conectado a una partida";
		retMsj["code"] = 1;
	}else{
		if (! data["nivel"].isNumeric()) {
			// TODO: Error
		}

		int nivel = data["nivel"].asInt();

		if(nivel > userData["nivel"].asInt()){
			retMsj["msj"] = "Tu nivel es bajo";
		}else{
			retMsj["msj"] = "Conectado satisfactoriamnte";
			retMsj["code"] = 0;
			Json::Value::Members keys = userData.getMemberNames();
				Json::Value yo;
				for(int i=0; i < keys.size(); i++){
					if(keys[i] != string("pass"))
						yo[keys[i]] = userData[keys[i]];
				}
			std::string nombre = data["nombre"].asString();
			this->partida = this->server->newPartida(nivel, nombre);
			this->partida->addUsuario(this, yo);
		}
	}

	if (this->write(retMsj)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de nueva partida");
		return 1;
	}
	return ret;
}

int ThreadUsuario::onGetMaps(Json::Value& data, Json::Value& userData){
	Value retMsj;
	int ret = 0;
	retMsj["event"] = EVENT_GET_MAPS;
	retMsj["msj"] = "Ok";
	retMsj["code"] = 0;
	int nivel = -1;
	if(data["nivel"].asInt())
		nivel = data["nivel"].asInt();

	retMsj["mapas"] = Listador::listar(nivel);

	if(this->write(retMsj)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de nueva partida");
		return 1;
	}
	return ret;
}

const std::string& ThreadUsuario::getUser(){
	return this->user;
}

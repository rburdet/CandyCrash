#include "server.thread_usuario.h"

#include <jsoncpp/json/json.h>
#include <string>
#include <sstream>

#include "common.logger.h"
#include "common.user_manager.h"

using Json::Value;
using Json::StaticString;
using std::string;
using std::stringstream;

ThreadUsuario::ThreadUsuario(ServerInterface* s, SocketIO* fd) : ThreadSocket(fd), server(s), partida(NULL), user("") {}

ThreadUsuario::~ThreadUsuario(){
	Logger::log("["+this->myId+"] Cerrando thread");
	if(this->partida)
		this->partida->rmUsuario(this);
}


void* ThreadUsuario::subRun(){
	if(this->welcome())
		return NULL;

	int ret_read;
	while(!(ret_read = this->read(false)));

	if(ret_read == -1){
		Logger::log("["+this->myId+"] Escuchando evento cifrado");

		while(! this->read());
	}

	Logger::log("["+this->myId+"] Termino coneccion");
	this->server->removeClient(this);

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

			this->user = data.get("user", def).asString();
			this->key = data.get("pass", def).asString();

			Logger::log("["+this->myId+"] Evento Login '"+this->user+"' '"+this->key+"'");
			Value userData;
			connect_msje["event"] = (int) EVENT_LOGIN;
			UserManager::get(this->user, userData);
			if(userData["user"] != this->user || userData["pass"] != this->key){
				Logger::log("["+this->myId+"] Error Login ");

				connect_msje["msj"] = "login error";
				connect_msje["code"] = 1;
				ret = 0;
			}else{
				Value ud;
				ud["user"] = userData["user"];
				ud["nivel"] = userData["nivel"];
				connect_msje["msj"] = "login correcto";
				connect_msje["code"] = 0;
				connect_msje["user"] = ud;
				ret = -1;
			}

			if(this->write(connect_msje)){
				Logger::log("["+this->myId+"] Error escribiendo el mensaje de confirmacion de login");
				return 1;
			}
			return ret;
			break;
		}

		case EVENT_NEW_USER:{ // TODO: se podria pasar a una funcion
			Value retMsj;
			int ret = 0;
			retMsj["event"] = EVENT_NEW_USER;

			this->user = data.get("user", def).asString();
			this->key = data.get("pass", def).asString();
			Logger::log("["+this->myId+"] Evento new user '"+this->user+"' '"+this->key+"'");

			Value userData;
			UserManager::get(this->user, userData);
			if(userData.isNull() && this->user != string("") && this->key != string("")){ // No existe usuario, se puede crear
				Value newUser;
				newUser["user"] = this->user;
				newUser["pass"] = this->key;
				newUser["nivel"] = 1;
				UserManager::set(newUser);

				retMsj["msj"] = "usuario creado correctamente";
				retMsj["code"] = 0;
				retMsj["user"] = this->user;;
				retMsj["pass"] = this->key;
				ret = -1;
			}else{
				retMsj["msj"] = "error creando usuario";
				retMsj["code"] = 1;
				ret = 0;
			}

			if(this->write(retMsj)){
				Logger::log("["+this->myId+"] Error escribiendo el mensaje de confirmacion de login");
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
				Logger::log("["+this->myId+"] Error escribiendo el mensaje de nueva partida");
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

		case EVENT_GAME_MISC:
			if(this->partida && this->partida->mensaje(data)){
				this->partida = NULL;
			}
			return 0;
			break;

		case EVENT_GET_MAPS:
			return this->onGetMaps(data, userData);
			break;

		default:
			Logger::log("["+this->myId+"] Evento desconocido");
			break;
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

		if(part){
			this->partida = part;
			part->addUsuario(this, user);
			// Todo: mandar mas data
			retMsj["msj"] = "Coneccion correcta";
			retMsj["code"] = 0;
		}
	}

	if(this->write(retMsj)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de conexion de partida");
		return 1;
	}
	return ret;
}

int ThreadUsuario::onNewGame(Json::Value& data, Json::Value& userData){
	Value retMsj;
	int ret = 0;
	retMsj["event"] = EVENT_NEW_GAME;
	retMsj["msj"] = "Ok";
	retMsj["code"] = 0;

	if(this->partida){
		retMsj["msj"] = "Ya estas conectado a una partida";
		retMsj["code"] = 1;
	}else{
		if(! data["nivel"].isNumeric()) {
			// TODO: Error
		}

		int nivel = data["nivel"].asInt();

		if(nivel > userData["nivel"].asInt()){
			// TODO: error
		}

		this->partida = this->server->newPartida(nivel);
		this->partida->addUsuario(this, this->user);
	}

	if(this->write(retMsj)){
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

	if(this->write(retMsj)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de nueva partida");
		return 1;
	}
	return ret;
}

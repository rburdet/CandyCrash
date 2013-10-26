#include "server.thread_usuario.h"

#include <jsoncpp/json/json.h>
#include <sstream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include "common.logger.h"
#include "common.user_manager.h"

using Json::Value;
using Json::StaticString;
using std::stringstream;

ThreadUsuario::ThreadUsuario(ServerInterface* s, SocketIO* fd) : server(s), fd(fd), user(""), pass(""), myId("") {}

ThreadUsuario::~ThreadUsuario(){
	Logger::log("["+this->myId+"] Cerrando thread");
	delete this->fd;
}

int ThreadUsuario::shutdown(){
	return this->fd->shutdown();
}

int ThreadUsuario::shutdown(int how){
	return this->fd->shutdown(how);
}

void* ThreadUsuario::run(){

	{
		stringstream ss;
		ss << (uint64_t) pthread_self();
		ss >> this->myId;
	}

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

	// XXX: me auto elmino?, vale esto?,, jajaja
	delete this;

	return NULL;
}

int ThreadUsuario::welcome(){
	Value connect_msje;
	connect_msje["msj"] = "conectado";
	connect_msje["code"] = 0;

	if(this->fd->write(connect_msje, this->pass)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de bienvenida");
		return 1;
	}
	return 0;
}

int ThreadUsuario::read(bool check){
	Value get_data;

	if(fd->read(get_data, this->pass, check)){
		Logger::log("["+this->myId+"] Error recibiendo informacion");
		return 1;
	}

	int ret = 0;

	if(check)
		return this->eventFirmado(get_data);
	else
		return this->eventNoFirmado(get_data);

	return ret;
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
			this->pass = data.get("pass", def).asString();

			Logger::log("["+this->myId+"] Evento Login '"+this->user+"' '"+this->pass+"'");
			Value userData;
			UserManager::get(this->user, userData);
			if(userData["user"] != this->user || userData["pass"] != this->pass){
				Logger::log("["+this->myId+"] Error Login ");

				connect_msje["event"] = (int) EVENT_LOGIN;
				connect_msje["msj"] = "login error";
				connect_msje["code"] = "1";
				ret = 0;
			}else{
				connect_msje["event"] = (int) EVENT_LOGIN;
				connect_msje["msj"] = "login correcto";
				connect_msje["code"] = "0";
				ret = -1;
			}

			if(this->fd->write(connect_msje, this->pass)){
				Logger::log("["+this->myId+"] Error escribiendo el mensaje de confirmacion de login");
				return 1;
			}
			return ret;
			break;
		 }

		case EVENT_NEW_USER:
			Logger::log("["+this->myId+"] Evento new user");
			break;

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

	switch(event){
		default:
			Logger::log("["+this->myId+"] Evento desconocido");
			break;
	}

	return 0;
}

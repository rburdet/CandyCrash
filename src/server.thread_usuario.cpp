#include "server.thread_usuario.h"

#include <jsoncpp/json/json.h>
#include <sstream>
#include <string>
#include <pthread.h>

#include "common.logger.h"

using Json::Value;
using Json::StaticString;
using std::stringstream;

ThreadUsuario::ThreadUsuario(SocketIO* fd) : fd(fd), user(""), pass(""), myId("") {}

ThreadUsuario::~ThreadUsuario(){
	delete this->fd;
}

void* ThreadUsuario::run(){
	Value connect_msje;
	Json::StaticString def("");

	{
		stringstream ss;
		ss << (int) pthread_self(); 
		ss >> this->myId;
	}

	connect_msje["msj"] = "conectado";
	connect_msje["code"] = 0;

	if(this->fd->write(connect_msje, this->pass)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de bienvenida");
		return NULL;
	}

	Value get_data;
	if(fd->read(get_data, this->pass, false)){
		Logger::log("["+this->myId+"] Error recibiendo usuario y contrase~na");
		return NULL;
	}

	this->user = get_data.get("user", def).asString();
	this->pass = get_data.get("pass", def).asString();

	// TODO: Comprobar usuario y contrase~na

	Logger::log("["+this->myId+"] Login correcto: '"+this->user+"'");

	connect_msje["msj"] = "login correcto";
	connect_msje["code"] = "0";

	if(this->fd->write(connect_msje, this->pass)){
		Logger::log("["+this->myId+"] Error escribiendo el mensaje de confirmacion de login");
		return NULL;
	}



	Logger::log("["+this->myId+"] Termino coneccion");
	return NULL;
}

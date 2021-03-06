#include "cliente.thread_listener.h"

#include <jsoncpp/json/json.h>
#include <string>

#include "../common/common.logger.h"

using Json::Value;
using Json::StaticString;
using std::string;
using std::endl;

ThreadListener::ThreadListener(ClienteInterface* s, SocketIO* fd) 
	: ThreadSocket(fd), cliente(s), sendFakeMsj(true) {}

ThreadListener::~ThreadListener(){
	Logger::log("["+this->myId+"] Cerrando thread");
}

void* ThreadListener::subRun(){
	this->read(false);
	Logger::log("["+this->myId+"] Escuchando evento cifrado");

	while(! this->read()){}

	Logger::log("["+this->myId+"] Termino coneccion");
	if(sendFakeMsj){
		Value fakePaquete;

		fakePaquete["code"] = 1;
		fakePaquete["msj"] = "Error en comunicarse con el servidor";
		fakePaquete["event"] = EVENT_LOGOUT;

		cliente->nuevoMensaje(fakePaquete);
	}

	return NULL;
}

int ThreadListener::eventNoFirmado(Value& data){
	Logger::log("["+this->myId+"] Evento no firmado. Deberia solo ser el saludo.");
	Json::StyledWriter writer;
	string output = writer.write(data);
	std::cout << output << endl;
	return 0;
}

int ThreadListener::eventFirmado(Value& data){
	cliente->nuevoMensaje(data);
	//Json::StyledWriter writer;
	//string output = writer.write(data);
	//std::cout << output << endl;
	return 0;
}

void ThreadListener::setKey(std::string&key){
	this->key = key;
}

int ThreadListener::shutdown(int how){
	this->sendFakeMsj = false;
	return ThreadSocket::shutdown(how);
}

int ThreadListener::shutdown(){
	this->sendFakeMsj = false;
	return ThreadSocket::shutdown();
}



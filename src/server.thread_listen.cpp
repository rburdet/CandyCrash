#include "server.thread_listen.h"

#include <jsoncpp/json/json.h>
#include <sstream>
#include <string>
#include "common.logger.h"

using std::string;
using std::stringstream;

ThreadListen::ThreadListen() : port(0){}
ThreadListen::~ThreadListen(){}

int ThreadListen::listen(int port){
	int ret;
	this->port = port;
	stringstream ss;
	ss << "PUERTO " << this->port << ". ";

	if((ret = this->sock.listen(this->port))){
		Logger::log(ss.str()+"Error.");
		return ret;
	}

	Logger::log(ss.str()+"Abierto.");
	return 0;
}

int ThreadListen::shutdown(){
	return this->sock.shutdown();
}

int ThreadListen::shutdown(int how){
	return this->sock.shutdown(how);
}

void* ThreadListen::run(){
	SocketIO* fd;
	stringstream ss;
	ss << "PUERTO " << this->port;

	Json::Value connect_msj;
	Json::Reader reader;

	reader.parse("{\"msj\":\"conectado\"}", connect_msj, false);

	while((fd = this->sock.accept())){
		if(fd->write(connect_msj, "")){
			Logger::log(ss.str()+". Error escribiendo mensaje de bienvenida.");
		}else{
			Json::Value root;
			Logger::log(ss.str()+". Conexión aceptada.");
			string msg = "";
			if(!fd->read(root, msg, false)){
				Json::StaticString def("default");
				Json::Value user = root.get("user", def);
				Json::Value pass = root.get("pass", def);

				stringstream ll;
				string ans = "Usuario '"+user.asString()+"' pass: '"+pass.asString()+"'";
				Logger::log(ans);

				Json::Value send_data;
				send_data["msj"] = string("Autenticacion correcta");
				send_data["code"] = 400;
				if(fd->write(send_data, pass.asString())){
					Logger::log("Errorrorororor");
				}

			}
		}

		Logger::log(ss.str()+". Conexión cerrada.");
		delete fd;
	}

	return NULL;
}


#include "server.thread_listen.h"

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

	while((fd = this->sock.accept())){
		if(fd->write(ss.str()+" Aceptado. Recibiendo datos...")){
			Logger::log(ss.str()+". Error escribiendo mensaje de bienvenida.");
		}else{
			Logger::log(ss.str()+". Conexión aceptada.");
			string msg;
			if(!fd->read(msg)){
				stringstream ll;
				string ans = "Datos recibidos exitosamente. Cantidad de bytes recibidos: ";
				ll << msg.length();
				if(fd->write(ans+ll.str()+"."))
					Logger::log(ss.str()+". Error escribiendo mensaje de verificacion.");
				Logger::log(ss.str()+". Recibidos "+ll.str()+" bytes.");
			}
		}

		Logger::log(ss.str()+". Conexión cerrada.");
		delete fd;
	}

	return NULL;
}


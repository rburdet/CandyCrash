#include "server.server.h"
#include "common.logger.h"

Server::Server(int port) : port(port) {
}


// TODO: eliminar clientes conectados
Server::~Server() {
}

int Server::main(){
	if(this->sock.listen(this->port)){
		Logger::log(" Error poniendo el socket a la escucha");
		return 1;
	}

	Logger::log("Escuchando en puerto especificado");

	SocketIO* fd;
	while( (fd = this->sock.accept()) ){
		//TODO: guardar Threadusuario
		ThreadUsuario* cli =  new ThreadUsuario(this, fd);
		cli->start();
		this->addClient(cli);
	}

	return 0;
}

void Server::addClient(ThreadUsuario* cli){
	this->clientesLock.lock();
	this->clientes.push_back(cli);
	this->clientesLock.unlock();
}

void Server::removeClient(Thread* cli){
	this->clientesLock.lock();

	for(unsigned int i=0; i < clientes.size(); i++){
		if((void*) cli == (void*) (this->clientes[i])){
			this->clientes.erase(this->clientes.begin() + i);
			break;
		}
	}

	this->clientesLock.unlock();
}

#include "server.server.h"
#include "common.logger.h"

Server::Server(int port) : port(port) {
}


Server::~Server() {
}

int Server::main(){
	if(this->sock.listen(this->port)){
		Logger::log(" Error poniendo el socket a la escucha");
		return 1;
	}

	Logger::log("Escuchando en puerto especificado");

	SocketIO* fd;
	// TODO: fijarse como implementar con select
	while( (fd = this->sock.accept()) ){
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

void Server::end(){
	this->clientesLock.lock();
	for(unsigned int i=0; i < clientes.size(); i++){
		this->clientes[i]->shutdown();
		this->clientes[i]->join();
		delete this->clientes[i];
		this->clientes.erase(this->clientes.begin() + i);
	}
	this->clientesLock.unlock();
}

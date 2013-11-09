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

PartidaInterface* Server::newPartida(int nivel){
	Partida *p = new Partida(this, nivel);
	this->partidasLock.lock();
	this->partidas.push_back(p);
	this->partidasLock.unlock();
	return p;
}

void Server::removePartida(PartidaInterface* p){
	this->partidasLock.lock();

	for(unsigned int i=0; i < partidas.size(); i++){
		if((void*) p == (void*) (this->partidas[i])){
			this->partidas.erase(this->partidas.begin() + i);
			break;
		}
	}

	this->partidasLock.unlock();
}

void Server::listPartidas(int nivel, Json::Value& parts){
	this->partidasLock.lock();
	int i = 0;
	for(unsigned int j=0; j < this->partidas.size(); j++){
		if(this->partidas[j]->getNivel() <= nivel){
			//TODO:
			parts[i]["id"] = (uint32_t) this->partidas[j];
			parts[i]["nivel"] = this->partidas[j]->getNivel();
			i++;
		}
	}
	
	this->partidasLock.unlock();
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

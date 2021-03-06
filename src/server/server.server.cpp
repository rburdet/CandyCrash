#include "server.server.h"
#include <string>
#include "../common/common.logger.h"
#include <sstream>
#include <unistd.h>

using std::stringstream;

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
		cli->detach();
		this->addClient(cli);
	}

	return 0;
}

void* Server::run(){
	this->main();
	return NULL;
}

void Server::addClient(ThreadUsuario* cli){
	this->clientesLock.lock();
	this->clientes.push_back(cli);
	this->clientesLock.unlock();
}

void Server::removeClient(ThreadSocket* cli){
	this->clientesLock.lock();

	for(unsigned int i=0; i < clientes.size(); i++){
		if((void*) cli == (void*) (this->clientes[i])){
			this->clientes.erase(this->clientes.begin() + i);
			break;
		}
	}

	this->clientesLock.unlock();
}

PartidaInterface* Server::newPartida(int nivel, std::string& nombre){
	Partida *p = new Partida(this, nivel, nombre);
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
		if(this->partidas[j]->getEstado() == PARTIDA_ABIERTA && 
				this->partidas[j]->getNivel() <= nivel){
			//TODO:
			stringstream ss;
			ss << (long) this->partidas[j];
			parts[i]["id"] = ss.str();
			parts[i]["nivel"] = this->partidas[j]->getNivel();
			parts[i]["nombre"] = this->partidas[j]->getNombre();
			parts[i]["users"] = this->partidas[j]->getUsuarios();
			parts[i]["max_users"] = this->partidas[j]->getMaxUsuarios();
			i++;
		}
	}
	
	this->partidasLock.unlock();
}

PartidaInterface* Server::connectPartidas(long id){
	PartidaInterface* part = NULL;
	this->partidasLock.lock();
	for(unsigned int j=0; j < this->partidas.size(); j++){
		if((long) this->partidas[j] == id){
			part = this->partidas[j];
			break;
		}
	}

	this->partidasLock.unlock();

	return part;
}

void Server::end(){
	this->clientesLock.lock();
	for(unsigned int i=0; i < this->clientes.size(); i++){
		this->clientes[i]->shutdown();
	}
	this->clientesLock.unlock();
	this->sock.shutdown();
	this->join();

	// Tengo que esperar que se cierren los threads detacheados.
	// Esto es solo para que el valgrind salga bien, por que al cerrar 
	// la aplicacion se liberan los recursos.
	sleep(1);
}

bool Server::userConectado(std::string& user){
	this->clientesLock.lock();
	for(unsigned int i=0; i < clientes.size(); i++){
		if(user == this->clientes[i]->getUser()){
			this->clientesLock.unlock();
			return true;
		}
	}
	this->clientesLock.unlock();
	return false;
}

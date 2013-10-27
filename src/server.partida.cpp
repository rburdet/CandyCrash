#include "server.partida.h"

Partida::Partida(ServerInterface* server, int nivel) : server(server), nivel(nivel) { }

Partida::~Partida() {
	this->usuariosLock.lock();
	for(unsigned int i=0; i < this->usuarios.size(); i++){
		// Decirles que se bajo la partida

	}
	this->usuariosLock.unlock();
	this->server->removePartida(this);
}


void Partida::addUsuario(ThreadSocket* u){
	this->usuariosLock.lock();
	this->usuarios.push_back(u);
	// TODO: Dar msje de bievenida a la partida
	this->usuariosLock.unlock();
}

void Partida::rmUsuario(ThreadSocket* u){
	this->usuariosLock.lock();
	for(unsigned int i=0; i < usuarios.size(); i++){
		if((void*) u == (void*) (this->usuarios[i])){
			this->usuarios.erase(this->usuarios.begin() + i);
			break;
		}
	}
	this->usuariosLock.unlock();
}

int Partida::getNivel(){
	return this->nivel;
}

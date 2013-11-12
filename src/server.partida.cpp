#include "server.partida.h"
#include <jsoncpp/json/json.h>
#include "common.events.h"
#include "common.logger.h"

using std::string;
using Json::Value;
Partida::Partida(ServerInterface* server, int nivel) : server(server), nivel(nivel) { }

Partida::~Partida() {
	this->usuariosLock.lock();
	for(unsigned int i=0; i < this->usuarios.size(); i++){
		// Decirles que se bajo la partida

	}
	this->usuariosLock.unlock();
	this->server->removePartida(this);
}


void Partida::addUsuario(ThreadSocket* u, string& user){
	this->usuariosLock.lock();
	Value retMsj;
	retMsj["event"] = EVENT_GAME_MISC;
	retMsj["msj"] = "Se conecto "+user;
	retMsj["code"] = 0;
	// TODO: Dar msje de bievenida a la partida
	for(unsigned int j=0; j < this->usuarios.size(); j++){
		if(this->usuarios[j]->write(retMsj)){
			Logger::log("[Partida] Error broadcasteando mensjae de nuevo usuario");
		}
	}
	this->usuarios.push_back(u);
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

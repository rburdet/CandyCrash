#include "server.partida.h"
#include "common.events.h"
#include "common.logger.h"

using std::string;
using Json::Value;
Partida::Partida(ServerInterface* server, int nivel) : server(server), nivel(nivel), estado(PARTIDA_ABIERTA) { }

Partida::~Partida() {
	this->usuariosLock.lock();
	for(unsigned int i=0; i < this->usuarios.size(); i++){
		// Decirles que se bajo la partida

	}
	this->usuariosLock.unlock();
	this->server->removePartida(this);
}


void Partida::addUsuario(ThreadSocket* u, string& user){
	Value retMsj;
	retMsj["event"] = EVENT_GAME_MISC;
	retMsj["msj"] = "Se conecto "+user;
	retMsj["code"] = 0;

	this->usuariosLock.lock();
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
	Value retMsj;
	retMsj["event"] = EVENT_GAME_MISC;
	retMsj["msj"] = "Usuario desconectado (te debo el nombre)";
	retMsj["code"] = 0;

	this->usuariosLock.lock();
	for(unsigned int i=0; i < usuarios.size(); i++){
		if((void*) u == (void*) (this->usuarios[i])){
			this->usuarios.erase(this->usuarios.begin() + i);
		}else{
			if(this->usuarios[i]->write(retMsj)){
				Logger::log("[Partida] Error broadcasteando mensjae de nuevo usuario");
			}
		}
	}
	this->usuariosLock.unlock();
}

int Partida::getNivel(){
	return this->nivel;
}

int Partida::getUsuarios(){
	return this->usuarios.size();
}

int Partida::mensaje(Json::Value& m){
	return 0;
}


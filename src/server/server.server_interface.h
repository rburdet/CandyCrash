#ifndef __SERVER_INTERFACE_H__
#define __SERVER_INTERFACE_H__
#include <jsoncpp/json/json.h>
#include <string>

#include "../common/common.thread.h"
#include "server.partida_interface.h"

class ServerInterface {
	public:
		virtual void removeClient(ThreadSocket* cli) = 0;
		virtual PartidaInterface* newPartida(int nivel, std::string& nombre) = 0;
		virtual void removePartida(PartidaInterface* p) = 0;
		//TODO:
		virtual void listPartidas(int nivel, Json::Value& parts) = 0;
		virtual PartidaInterface* connectPartidas(long id) = 0;
		virtual bool userConectado(std::string& user) = 0;
};

#endif

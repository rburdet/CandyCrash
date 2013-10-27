#ifndef __SERVER_INTERFACE_H__
#define __SERVER_INTERFACE_H__
#include <jsoncpp/json/json.h>

#include "common.thread.h"
#include "server.partida_interface.h"

class ServerInterface {
	public:
		virtual void removeClient(Thread* cli) = 0;
		virtual PartidaInterface* newPartida(int nivel) = 0;
		virtual void removePartida(PartidaInterface* p) = 0;
		//TODO:
		virtual void listPartidas(int nivel, Json::Value& parts) = 0;
};

#endif

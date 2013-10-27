#ifndef __SERVER_THREAD_USUARIO_H__
#define __SERVER_THREAD_USUARIO_H__
#include <string>
#include <jsoncpp/json/json.h>

#include "server.server_interface.h"
#include "server.partida_interface.h"
#include "common.thread.h"
#include "common.socket_io.h"
#include "common.events.h"
#include "common.mutex.h"

class ThreadUsuario : public ThreadSocket {
	protected:
		ServerInterface* server;
		PartidaInterface* partida;
		std::string user;
		//std::string pass;

		virtual int eventNoFirmado(Json::Value& data);
		virtual int eventFirmado(Json::Value& data);
		virtual void* subRun();

		int welcome();

	public:
		ThreadUsuario(ServerInterface* s, SocketIO* fd);
		~ThreadUsuario();
};

#endif

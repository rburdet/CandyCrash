#ifndef __SERVER_THREAD_USUARIO_H__
#define __SERVER_THREAD_USUARIO_H__
#include <string>
#include <jsoncpp/json/json.h>

//#include "server.server.h"
#include "server.server_interface.h"
#include "common.thread.h"
#include "common.socket_io.h"
#include "common.events.h"

class ThreadUsuario : public Thread {
	protected:
		virtual void* run();
		ServerInterface* server;
		SocketIO* fd;
		std::string user;
		std::string pass;
		std::string myId;

		int read(bool check=true);
		int welcome();

		int eventNoFirmado(Json::Value& data);
		int eventFirmado(Json::Value& data);

	public:
		ThreadUsuario(ServerInterface* s, SocketIO* fd);
		~ThreadUsuario();
};

#endif

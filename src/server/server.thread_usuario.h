#ifndef __SERVER_THREAD_USUARIO_H__
#define __SERVER_THREAD_USUARIO_H__
#include <string>
#include <jsoncpp/json/json.h>

#include "server.server_interface.h"
#include "server.partida_interface.h"
#include "../common/common.thread.h"
#include "../common/common.socket_io.h"
#include "../common/common.events.h"
#include "../common/common.mutex.h"

/** Thread usado para cada usuario que se conecta
 */
class ThreadUsuario : public ThreadSocket {
	protected:
		ServerInterface* server;
		PartidaInterface* partida;
		std::string user;
		//std::string pass;

		virtual int eventNoFirmado(Json::Value& data);
		virtual int eventFirmado(Json::Value& data);
		virtual void* subRun();

		int onJoinGame(Json::Value& data, Json::Value& userData);
		int onLeaveGame(Json::Value& data, Json::Value& userData);
		int onNewGame(Json::Value& data, Json::Value& userData);
		int onGetMaps(Json::Value& data, Json::Value& userData);

		int welcome();

	public:
		ThreadUsuario(ServerInterface* s, SocketIO* fd);
		~ThreadUsuario();
};

#endif

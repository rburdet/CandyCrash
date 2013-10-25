#ifndef __SERVER_THREAD_USUARIO_H__
#define __SERVER_THREAD_USUARIO_H__
#include <string>

#include "common.thread.h"
#include "common.socket_io.h"

class ThreadUsuario : public Thread {
	protected:
		virtual void* run();
		SocketIO* fd;
		std::string user;
		std::string pass;
		std::string myId;

	public:
		ThreadUsuario(SocketIO* fd);
		~ThreadUsuario();
};

#endif

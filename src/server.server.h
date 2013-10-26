#ifndef __SERVER_SERVER_H__
#define __SERVER_SERVER_H__

#include <vector>
#include <string>

#include "server.server_interface.h"
#include "common.thread.h"
#include "server.thread_usuario.h"
#include "server.socket_listener.h"
#include "common.mutex.h"

class Server : public ServerInterface {
	protected:
		int port;
		std::vector<ThreadUsuario*> clientes;
		TCPSocketListener sock;
		Mutex clientesLock;

		void addClient(ThreadUsuario* cli);

		//ThreadUsuario* getClient(std::string usuario);

	public:
		Server(int port);
		~Server();

		int main();
		void removeClient(Thread* cli);
		void end();
};

#endif

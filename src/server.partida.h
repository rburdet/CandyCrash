#ifndef __SERVER_PARTIDA_H__
#define __SERVER_PARTIDA_H__
#include <vector>

#include "server.server_interface.h"
#include "server.partida_interface.h"
#include "server.thread_socket.h"
#include "common.mutex.h"

class Partida : public PartidaInterface {
	protected:
		ServerInterface* server;
		std::vector<ThreadSocket*> usuarios;
		int nivel;
		Mutex usuariosLock;


	public:
		Partida(ServerInterface* server, int nivel);
		~Partida();

		void addUsuario(ThreadSocket* u);
		void rmUsuario(ThreadSocket* u);

		int getNivel();

};


#endif

#ifndef __SERVER_PARTIDA_H__
#define __SERVER_PARTIDA_H__
#include <vector>

#include "server.server_interface.h"
#include "server.partida_interface.h"
#include "common.thread_socket.h"
#include "common.mutex.h"

typedef enum {
	PARTIDA_ABIERTA,
	PARTIDA_JUGANDO
} PartidaEstado;

class Partida : public PartidaInterface {
	protected:
		ServerInterface* server;
		std::vector<ThreadSocket*> usuarios;
		int nivel;
		Mutex usuariosLock;
		PartidaEstado estado;


	public:
		Partida(ServerInterface* server, int nivel);
		~Partida();

		void addUsuario(ThreadSocket* u, std::string& user);
		void rmUsuario(ThreadSocket* u);

		int getNivel();
		int getUsuarios();

		virtual int mensaje(Json::Value& m);

};


#endif

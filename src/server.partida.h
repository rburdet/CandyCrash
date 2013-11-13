#ifndef __SERVER_PARTIDA_H__
#define __SERVER_PARTIDA_H__
#include <vector>

#include "server.server_interface.h"
#include "server.partida_interface.h"
#include "common.thread_socket.h"
#include "common.mutex.h"

class Partida : public PartidaInterface {
	protected:
		ServerInterface* server;
		std::vector<ThreadSocket*> usuarios;
		int nivel;
		std::string nombre;
		Mutex usuariosLock;
		PartidaEstado estado;
		Json::Value mapa;
		int maxUsuarios;
		void broadcastMsj(Json::Value& msj);


	public:
		Partida(ServerInterface* server, int nivel, std::string& nombre);
		~Partida();

		void addUsuario(ThreadSocket* u, std::string& user);
		void rmUsuario(ThreadSocket* u);

		int getNivel();
		int getUsuarios();
		int getMaxUsuarios();
		PartidaEstado getEstado();
		std::string getNombre();

		virtual int mensaje(Json::Value& m);

};


#endif

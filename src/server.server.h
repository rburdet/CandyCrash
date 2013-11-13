#ifndef __SERVER_SERVER_H__
#define __SERVER_SERVER_H__

#include <vector>
#include <string>

#include "server.server_interface.h"
#include "common.thread.h"
#include "server.thread_usuario.h"
#include "server.socket_listener.h"
#include "common.mutex.h"
#include "server.partida.h"

class Server : public ServerInterface {
	protected:
		int port;
		std::vector<ThreadUsuario*> clientes;
		std::vector<Partida*> partidas;
		TCPSocketListener sock;
		Mutex clientesLock;
		Mutex partidasLock;

		void addClient(ThreadUsuario* cli);

		//ThreadUsuario* getClient(std::string usuario);

	public:
		Server(int port);
		~Server();

		int main();
		void removeClient(ThreadSocket* cli);
		PartidaInterface* newPartida(int nivel, std::string& nombre);
		void removePartida(PartidaInterface* p);
		//TODO:
		virtual void listPartidas(int nivel, Json::Value& parts);
		virtual PartidaInterface* connectPartidas(long id);
		void end();
};

#endif

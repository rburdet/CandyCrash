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

/** Clase de servidor.
 * Esta bloqueada escuchando, cada vez que se conecta un cliente se lanza un nuevo thread con el socket creado (se lanza un ThreadUsuario)
 */
class Server : public ServerInterface {
	protected:
		int port;

		/** Guarda todos los threads de los usuarios corriendo
		 */
		std::vector<ThreadUsuario*> clientes;
		std::vector<Partida*> partidas; // -> Partidas activas
		TCPSocketListener sock; // -> socket usado para la recepcion de conecciones
		Mutex clientesLock; // -> Mutex para el std::vector de los usuarios
		Mutex partidasLock; // Mutex para el std::Vector de las partidas

		void addClient(ThreadUsuario* cli);

		//ThreadUsuario* getClient(std::string usuario);

	public:
		Server(int port);
		~Server();

		int main();
		void removeClient(ThreadSocket* cli);
		PartidaInterface* newPartida(int nivel, std::string& nombre);

		/** Remueve partida de la lista
		 */
		void removePartida(PartidaInterface* p);

		/** Devuelve un JSON con las partidas de nivel menor o igual al especificado.
		 */
		virtual void listPartidas(int nivel, Json::Value& parts);

		/** 
		 */
		virtual PartidaInterface* connectPartidas(long id);
		void end();
};

#endif

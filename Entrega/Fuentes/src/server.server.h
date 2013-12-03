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
class Server : public ServerInterface, public Thread {
	protected:
		int port;

		/** Guarda todos los threads de los usuarios corriendo
		 */
		std::vector<ThreadUsuario*> clientes;
		/** Partidas activas
		 */
		std::vector<Partida*> partidas;
		/** Socket usado para la recepccion de conneciones
		 */
		TCPSocketListener sock;
		/** Mutex para el std::vector de los usuarios
		 */
		Mutex clientesLock;
		/** Mutex para el std::Vector de las partidas
		 */
		Mutex partidasLock;

		void addClient(ThreadUsuario* cli);

		/** main del servidor, es el que escucha y despacha a los clientes, se ejecuta en un thread aparte.
		 */
		int main();
		//ThreadUsuario* getClient(std::string usuario);

		virtual void* run();

	public:
		explicit Server(int port);
		~Server();

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
		/** Finaliza el servidor
		 */
		void end();
};

#endif

#ifndef __SERVER_PARTIDA_H__
#define __SERVER_PARTIDA_H__
#include <vector>

#include "server.server_interface.h"
#include "server.partida_interface.h"
#include "common.thread_socket.h"
#include "common.mutex.h"

/** Entidad que representa una partida.
 */
class Partida : public PartidaInterface {
	protected:
		ServerInterface* server;
		std::vector<ThreadSocket*> usuarios;
		std::vector<Json::Value> usuarios_data;
		int nivel;
		std::string nombre;
		Mutex usuariosLock;
		PartidaEstado estado;
		Json::Value mapa;
		int maxUsuarios;
		void broadcastMsj(Json::Value& msj);


	public:
		/** Creador de la partida.
		 * @param server[in]: instancia de servidor
		 * @param nivel[in]: nivel de la partida
		 * @param nombre[in]: nombre del mapa
		 */
		Partida(ServerInterface* server, int nivel, std::string& nombre);
		~Partida();

		/** Agrega un usuario a la partida.
		 * @param u[in]: threadsocket del usuario
		 * @param user[in]: nombre de usuario
		 */
		void addUsuario(ThreadSocket* u, Json::Value& user);

		/** Borra un usuario de la partida
		 * @param u[in]: threasocket del usuario
		 */
		void rmUsuario(ThreadSocket* u);

		/** Metodos que devuelven informacion acerca de la partida
		 */
		int getNivel();
		int getUsuarios();
		int getMaxUsuarios();
		PartidaEstado getEstado();
		std::string getNombre();

		/** Metodo usado para mandarle mensajes a la partida.
		 * @param m[in]: mensaje encondeado en JSON que recibe la partida
		 * @param u[in]: ThreadSocket que le envio el mensaje
		 */
		virtual int mensaje(Json::Value& m, ThreadSocket* u);

};


#endif

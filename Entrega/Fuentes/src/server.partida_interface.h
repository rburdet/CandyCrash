#ifndef __SERVER_PARTIDA_INTERFACE_H__
#define __SERVER_PARTIDA_INTERFACE_H__

#include <string>
#include "common.thread_socket.h"
#include <jsoncpp/json/json.h>

/** Estado de la partida
 */
typedef enum {
	PARTIDA_ABIERTA=0,
	PARTIDA_JUGANDO,
	PARTIDA_TERMINADA
} PartidaEstado;

/** Interface de partida, para evitar referencia circular.
 */
class PartidaInterface {
	public:
		/** Agrega un usuario a l apartida.
		 * @param u[in] ThreadSocket del usuario que se conecta
		 * @param user[in] informacion acerca del usuario que se conecta
		 */
		virtual void addUsuario(ThreadSocket* u, Json::Value& user) = 0;
		/** Remueve un usuario de la partida.
		 * @param u[in] ThreadSocket del usuario que se conecta
		 */
		virtual void rmUsuario(ThreadSocket* u) = 0;
		/** Se le pasan todos los  EVENT_GAME_MISC. Devuelve 0 si todo bien, o 
		 * 1, si el usuario se desconecto (saca usuario->partida). La parida se
		 * debe encargar de toda la borrada de datos de si misma.
		 */
		virtual int mensaje(Json::Value& m, ThreadSocket* u) = 0;
		virtual int getNivel() = 0;
		virtual int getUsuarios() = 0;
		virtual int getMaxUsuarios() = 0;
		virtual PartidaEstado getEstado() = 0;
		virtual std::string getNombre() = 0;
};

#endif

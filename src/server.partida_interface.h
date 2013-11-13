#ifndef __SERVER_PARTIDA_INTERFACE_H__
#define __SERVER_PARTIDA_INTERFACE_H__
#include "common.thread_socket.h"
#include <jsoncpp/json/json.h>

typedef enum {
	PARTIDA_ABIERTA=0,
	PARTIDA_JUGANDO
} PartidaEstado;

class PartidaInterface {
	public:
		virtual void addUsuario(ThreadSocket* u, std::string& user) = 0;
		virtual void rmUsuario(ThreadSocket* u) = 0;
		/** Se le pasan todos los  EVENT_GAME_MISC. Devuelve 0 si todo bien, o 1, si el usuario se desconecto (saca usuario->partida). La parida se debe encargar de toda la borrada de datos de si misma.
		 */
		virtual int mensaje(Json::Value& m) = 0;

};

#endif

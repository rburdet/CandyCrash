#ifndef __SERVER_PARTIDA_INTERFACE_H__
#define __SERVER_PARTIDA_INTERFACE_H__
#include "common.thread_socket.h"

class PartidaInterface {
	public:
		virtual void addUsuario(ThreadSocket* u) = 0;
		virtual void rmUsuario(ThreadSocket* u) = 0;

};

#endif

#ifndef __SERVER_SOCKET_LISTENER_H__
#define __SERVER_SOCKET_LISTENER_H__
#include "common.socket.h"
#include "common.socket_io.h"
#include <string>

class TCPSocketListener : public TCPSocket {
	protected:
		unsigned int backlog;

	public:
		TCPSocketListener();
		~TCPSocketListener();

		/**Escucha a una nueva conexion.
		 */
		int listen(const int port);
		int listen(const int port, const std::string& ip);
		int listen(struct sockaddr_in & serv_addr);
		/** Acepta una nueva conexion, devuelve un file descriptor. Bloqueante */
		SocketIO* accept();
		TCPSocketListener& setBacklog(const unsigned int backlog);
};

#endif

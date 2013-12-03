#ifndef __CLIENTE_SOCKET_CONNECT_H__
#define __CLIENTE_SOCKET_CONNECT_H__
#include "common.socket.h"
#include "common.socket_io.h"
#include <string>

/** Socket TCP que permite hacer una coneccion.
 */
class TCPSocketConnect : public TCPSocket, public SocketIO {
	//using TCPSocket::fd;
	public:
		TCPSocketConnect();
		~TCPSocketConnect();

		int connect(const std::string& ip);
		int connect(const int port, const std::string& ip);
		int connect(struct sockaddr_in & serv_addr);
};

#endif

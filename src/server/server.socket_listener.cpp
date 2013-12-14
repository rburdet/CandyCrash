#include "server.socket_listener.h"
#include <stdlib.h>
#include <string.h>
#include <string>

TCPSocketListener::TCPSocketListener() : backlog(5){}
TCPSocketListener::~TCPSocketListener() {}

int TCPSocketListener::listen(const int port){
	struct sockaddr_in serv_addr;
	memset((char*) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	return this->listen(serv_addr);
}

int TCPSocketListener::listen(const int port, const std::string& ip){
	struct sockaddr_in* addr = this->ip2struct(port, ip);
	int ret = this->listen(*addr);
	free(addr);
	return ret;
}

int TCPSocketListener::listen(struct sockaddr_in & serv_addr){
	int optval = 1;
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &optval,
				sizeof(int)))
		return -1;

	if ( bind( this->fd, (struct sockaddr *) &serv_addr,
			(socklen_t) sizeof(struct sockaddr)))
		return -1;

	return ::listen(this->fd, this->backlog);
}

SocketIO* TCPSocketListener::accept(){
	struct sockaddr_in cli_addr;
	unsigned int clilen = (socklen_t) sizeof(cli_addr);
	int ret = ::accept(this->fd, (struct sockaddr *) &cli_addr, &clilen);
	if(ret == -1)
		return NULL;
	return new SocketIO(ret);
}

TCPSocketListener& TCPSocketListener::setBacklog(const unsigned int backlog){
	this->backlog = backlog;
	return *this;
}

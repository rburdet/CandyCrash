#include "cliente.socket_connect.h"

#include <stdlib.h>
#include <string>

using std::string;

TCPSocketConnect::TCPSocketConnect() : TCPSocket(), SocketIO(){}
TCPSocketConnect::~TCPSocketConnect(){}

int TCPSocketConnect::connect(const string& ip){
	struct sockaddr_in* addr = this->ip2struct(ip);

	if(!addr)
		return -1;

	int ret = this->connect(*addr);
	free(addr);
	return ret;
}
int TCPSocketConnect::connect(const int port, const std::string& ip){
	struct sockaddr_in* addr = this->ip2struct(port, ip);

	if(!addr)
		return -1;

	int ret = this->connect(*addr);
	free(addr);
	return ret;
}

int TCPSocketConnect::connect(struct sockaddr_in & serv_addr){
	return ::connect(this->fd, (struct sockaddr *) &serv_addr,
			(socklen_t) sizeof(struct sockaddr));
}


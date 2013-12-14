#include "common.socket.h"
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

using std::string;
using std::stringstream;
using std::size_t;

Socket::Socket() : fd(0){}
Socket::~Socket(){
	if(this->fd){
		close(fd);
	}
}

int Socket::shutdown(){
	return this->shutdown(SHUT_RDWR);
}

int Socket::shutdown(int how){
	return ::shutdown(this->fd, how);
}

struct sockaddr_in* Socket::ip2struct(const string& ipport){
	string ip, service;
	size_t found = ipport.find(':');
	if(found != std::string::npos){
		ip = ipport.substr(0, found);
		service = ipport.substr(found+1, ipport.length()-found);
		return this->ip2struct(service, ip);
	}
	return NULL;
}

struct sockaddr_in* Socket::ip2struct(const int port, const string& ip){
	struct sockaddr_in *serv_addr = (struct sockaddr_in *) calloc(1,
		sizeof(sockaddr_in));
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_port = htons(port);
	//serv_addr->sin_addr.s_addr = inet_addr(ip.c_str());
	inet_pton(AF_INET, ip.c_str(), &(serv_addr->sin_addr));
	return serv_addr;
}

struct sockaddr_in* Socket::ip2struct(const string& port, const string& ip){
	stringstream ss(port);
	int p;
	ss >> p;

	return this->ip2struct(p, ip);
}

TCPSocket::TCPSocket(){
	this->fd = socket(AF_INET, SOCK_STREAM, 0);
}

TCPSocket::~TCPSocket(){}

#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include <string>
#include <netdb.h>

class Socket {
	protected:
		unsigned int fd;
		struct sockaddr_in* ip2struct(const int port, const std::string& ip);
		struct sockaddr_in* ip2struct(const std::string& port, const std::string& ip);
		struct sockaddr_in* ip2struct(const std::string& ipport);
	public:
		Socket();
		~Socket();

		int shutdown();
		int shutdown(int how);
};

class TCPSocket : public virtual Socket {
	public:
		TCPSocket();
		~TCPSocket();
};



#endif

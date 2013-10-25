#ifndef __COMMON_SOCKET_IO_H__
#define __COMMON_SOCKET_IO_H__
#include <string>
#include "common.socket.h"

class SocketIO : public virtual Socket {
	public:
		SocketIO();
		explicit SocketIO(unsigned int fd);
		~SocketIO();

		int read(std::string &msg);
		int write(const std::string &msg);
};

#endif

#ifndef __COMMON_SOCKET_IO_H__
#define __COMMON_SOCKET_IO_H__
#include <string>
#include <jsoncpp/json/json.h>
#include "common.socket.h"

class SocketIO : public virtual Socket {
	public:
		SocketIO();
		explicit SocketIO(unsigned int fd);
		~SocketIO();

		int read(Json::Value &data, const std::string& key, bool check=true);
		int write(const Json::Value &data, const std::string& key);
};

#endif

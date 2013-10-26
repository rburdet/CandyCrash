#include "common.socket_io.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "common.hmac.h"

using std::string;
using std::stringstream;

SocketIO::SocketIO() {}
SocketIO::SocketIO(unsigned int fd) {
	this->fd = fd;
}
SocketIO::~SocketIO(){}

int SocketIO::read(Json::Value &data, const std::string& key, bool check){
	uint32_t len;
	string msg;

	if(recv(this->fd, (void*) &len, 4, 0) != 4)
		return -1;

	len = ntohl(len);

	char *str = new char[len];

	if(((uint32_t) recv(this->fd, (void*) str, len, 0)) != len){
		delete[] str;
		//std::cout << "error leyendo msje" << std::endl;
		return -1;
	}

	{
		stringstream ss;
		for(uint32_t i=0; i < len; i++)
			ss << str[i];

		msg = ss.str();
	}

	delete[] str;

	str = new char[HMAC_LENGTH];

	uint32_t hmac_size;
	if((hmac_size = (uint32_t) recv(this->fd, (void*) str, HMAC_LENGTH, 0)) != HMAC_LENGTH){
		//std::cout << "error leyendo HMAC '" << str << "' size " << hmac_size << " HMAC_LENGTH  " << HMAC_LENGTH <<  std::endl;
		delete[] str;
		return -1;
	}

	Json::Reader reader;
	if(! reader.parse(msg, data, false)){
		return 2;
	}

	if(check){
		string hmac_key_rec;
		string hmac_key;
		stringstream ss;

		for(uint32_t i=0; i < HMAC_LENGTH; i++)
			ss << str[i];

		hmac_key_rec = ss.str();
		hmac_msje(key, msg, hmac_key);

		if(hmac_key != hmac_key_rec){ // Fallo validacion
			//std::cout << "error verificacion" << std::endl;
			delete[] str;
			return 1;
		}
	}

	delete[] str;
	return 0;
}

int SocketIO::write(const Json::Value &data, const string& key){
	Json::FastWriter writer;
	string msg = writer.write(data);
	uint32_t len = htonl(msg.length());
	string hmac_key;

	hmac_msje(key, msg, hmac_key);

	if(send(this->fd, (void*) &len, 4, 0) != 4)
		return -1;

	if ((uint32_t) send(this->fd,
			(void*) msg.c_str(), msg.length(), 0) != msg.length())
		return -1;

	if ((uint32_t) send(this->fd,
			(void*) hmac_key.c_str(), hmac_key.length(), 0) != hmac_key.length())
		return -1;

	return 0;
}

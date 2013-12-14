#include "common.thread_socket.h"
#include <sstream>
#include <string>
#include <sstream>

#include <pthread.h>
#include <unistd.h>

#include "common.logger.h"

using std::stringstream;
using std::string;
using Json::Value;

ThreadSocket::ThreadSocket(SocketIO* fd) : fd(fd), myId(""), key("") {}

ThreadSocket::~ThreadSocket() {
	delete this->fd;
	this->fd = NULL;
}

int ThreadSocket::shutdown(){
	return this->fd->shutdown();
}

int ThreadSocket::shutdown(int how){
	return this->fd->shutdown(how);
}

void* ThreadSocket::run(){
	{
		stringstream ss;
		//ss << (uint64_t) pthread_self();
		ss << (uint64_t) this;
		ss >> this->myId;
	}

	this->subRun();

	// XXX: me auto elmino?, vale esto?,, jajaja
	delete this;

	return NULL;
}

int ThreadSocket::read(bool check){
	Value get_data;

	if(this->fd->read(get_data, this->key, check)){
		Logger::log("["+this->myId+"] Error recibiendo informacion");
		return 1;
	}

	int ret = 0;

	if(check)
		return this->eventFirmado(get_data);
	else
		return this->eventNoFirmado(get_data);

	return ret;
}

int ThreadSocket::write(Value data){
	this->writeMutex.lock();
	int ret = this->fd->write(data, this->key);
	this->writeMutex.unlock();
	return ret;
}

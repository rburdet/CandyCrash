#ifndef __CLIENTE_THREAD_LISTENER_H__
#define __CLIENTE_THREAD_LISTENER_H__
#include <string>
#include <jsoncpp/json/json.h>

#include "common.thread.h"
#include "common.socket_io.h"
#include "common.events.h"
#include "common.mutex.h"
#include "common.thread_socket.h"
#include "cliente.cliente_interface.h"

class ThreadListener : public ThreadSocket {
	protected:
		ClienteInterface* cliente;

		virtual int eventNoFirmado(Json::Value& data);
		virtual int eventFirmado(Json::Value& data);
		virtual void* subRun();

	public:
		ThreadListener(ClienteInterface* s, SocketIO* fd);
		~ThreadListener();
		void setKey(std::string&key);
};

#endif

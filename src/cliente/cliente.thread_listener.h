#ifndef __CLIENTE_THREAD_LISTENER_H__
#define __CLIENTE_THREAD_LISTENER_H__
#include <string>
#include <jsoncpp/json/json.h>

#include "../common/common.thread.h"
#include "../common/common.socket_io.h"
#include "../common/common.events.h"
#include "../common/common.mutex.h"
#include "../common/common.thread_socket.h"
#include "cliente.cliente_interface.h"

/** Thread que se encarga de leer constantemente al socket. De recibir algun mensaje lo agrega a la cola de mensajes del cliente.
 */
class ThreadListener : public ThreadSocket {
	protected:
		ClienteInterface* cliente;

		virtual int eventNoFirmado(Json::Value& data);
		virtual int eventFirmado(Json::Value& data);
		virtual void* subRun();

		bool sendFakeMsj;

	public:
		ThreadListener(ClienteInterface* s, SocketIO* fd);
		~ThreadListener();
		/** Setear la clave usada para la firma digital de los paquetes
		 */
		void setKey(std::string&key);

		int shutdown(int how);
		int shutdown();
};

#endif

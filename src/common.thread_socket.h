#ifndef __SERVER_THREAD_SOCKET_H__
#define __SERVER_THREAD_SOCKET_H__
#include <string>
#include "common.mutex.h"
#include "common.socket_io.h"
#include "common.thread.h"

/** Thread encargado del manejo de un socket.
 * Posee un metodo de escritura protegida con mutex, la lectura siempre se hace dentro del thread.
 */
class ThreadSocket : public Thread {
	protected:
		virtual void* run();
		virtual void* subRun() = 0;
		SocketIO* fd;
		Mutex writeMutex;
		std::string myId;
		std::string key;

		int read(bool check=true);
		virtual int eventNoFirmado(Json::Value& data) = 0;
		virtual int eventFirmado(Json::Value& data) = 0;

	public:
		ThreadSocket(SocketIO* fd);
		~ThreadSocket();

		int shutdown(int how);
		int shutdown();
		int write(Json::Value data);
};

#endif

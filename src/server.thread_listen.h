#ifndef __SERVER_THREAD_LISTEN_H__
#define __SERVER_THREAD_LISTEN_H__

#include "common.thread.h"
#include "server.socket_listener.h"

/**
 * Thread de escucha.
 * Thread que usa el servidor para atender las connecciones.
 */
class ThreadListen : public Thread {
	protected:
		TCPSocketListener sock;
		int port;
		virtual void* run();

	public:
		ThreadListen();
		~ThreadListen();

		/** listen: pone al socket a escuchar, es no bloqueante.
		 * No hace el accept, solo el listen.
		 * @param [in] port puerto a escuchar
		 * @return 0-> no error, sino la salida de error del listen.
		 */
		int listen(int port);

		/** shutdown: hace un shutdown en el socket.
		 * @param [in] how valor que se le pasa al shutdown.
		 * @return salida de error del shutdown del socket.
		 */
		int shutdown(int how);

		/** shutdown: hace un shutdown en el socket.
		 * Usando un valor por defecto.
		 * @see listen(int port)
		 * @return salida de error del shutdown del socket.
		 */
		int shutdown();
};
#endif

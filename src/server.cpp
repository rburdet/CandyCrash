#include <iostream>
#include <string>

//#include "server.thread_listen.h"
#include "server.thread_usuario.h"
#include "server.socket_listener.h"
//#include "common.lista.h"
#include "common.logger.h"

using std::cin;
using std::string;
using std::cout;
using std::endl;

/** str2number: transforma str a numero
 * @param [in] str puerto en string
 * @return numero
 */
int str2number(char* str);

/** threads_shutdown: manda se~nal de shutdown a los threads.
 * @param [in] threads lista de threads de escucha
 */
//void threads_shutdown(Lista<ThreadListen*> & threads);

/** threads_join: manda se~nal de join a los threads.
 * @param [in] threads lista de threads de escucha
 */
//void threads_join(Lista<ThreadListen*> & threads);

/** Main de server.
 * Recibe como unico argumento de argv: puerto de escucha.
 * Se puede cerrar haciendo click en q.
 * @param [in] argc cantidad de argumentos (0 es el nombre del programa)
 * @param [in] argv parametros.
 * @return codigo de error.
 */
int main(int argc, char* argv[]){
	if(argc != 2)
		return 1;

	int port = str2number(argv[1]);

	if(port == 0){
		cout << "Puerto ingresado invalido" << endl;
		return 1;
	}

	TCPSocketListener sock;

	if(sock.listen(port)){
		cout << "Error poniendo el socket a la escucha" << endl;
		return 1;
	}
	Logger::init();

	cout << "Servidor escuchando en puerto '" << port << "'" << endl;


	// TODO hacer select o algo para poder cerrar de manera linda
	
	SocketIO* fd;
	while( (fd = sock.accept()) ){
		//TODO: guardar Threadusuario
		ThreadUsuario* tu =  new ThreadUsuario(fd);
		tu->start();
	}

	//int errors;
	//char c;
	//errors = threads_create(threads, argv[1]);
	//while ( (c = cin.get()) != 'q') {}
	//threads_shutdown(threads);
	//threads_join(threads);

	Logger::destroy();

	//return errors;
	return 0;
}

int str2number(char* str){
	int port = 0;
	char c;

	for(int i=0; (c = str[i]); i++){
		switch(c){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				port = port * 10 + c - '0';
				break;

			default:
				break;
		}
	}

	return port;
}

/*
void threads_shutdown(Lista<ThreadListen*> & threads){
	ThreadListen* thread;
	~threads;

	if(!threads)
		return;

	do {
		threads >> thread;
		thread->shutdown();
	} while (threads++);
}

void threads_join(Lista<ThreadListen*> & threads){
	ThreadListen* thread;
	~threads;

	if(!threads)
		return;

	do {
		threads >> thread;
		thread->join();
		delete thread;
	} while (threads++);
}
*/

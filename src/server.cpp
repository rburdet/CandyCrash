#include <iostream>
#include <string>

#include "server.thread_listen.h"
#include "common.lista.h"
#include "common.logger.h"

using std::cin;
using std::string;

int threads_create(Lista<ThreadListen*> & threads, char* thread_str);
void threads_shutdown(Lista<ThreadListen*> & threads);
void threads_join(Lista<ThreadListen*> & threads);

int main(int argc, char* argv[]){
	if(argc != 2)
		return 1;

	int errors;
	Logger::init();

	Lista<ThreadListen*> threads;
	char c;

	errors = threads_create(threads, argv[1]);

	while ( (c = cin.get()) != 'q') {}

	threads_shutdown(threads);
	threads_join(threads);

	Logger::destroy();

	return errors;
}

int thread_create(Lista<ThreadListen*> & threads, int port){
	ThreadListen* thread = new ThreadListen();
	if(thread->listen(port)){
		delete thread;
		return 1;
	}

	thread->start();
	threads << thread;

	return 0;
}

int threads_create(Lista<ThreadListen*> & threads, char* thread_str){
	int port = 0;
	char c = 0;
	int errors = 0;

	for(int i=0; (c = thread_str[i]); i++){
		if(c == ',' && port){
			errors += thread_create(threads, port);
			port = 0;
		}else{
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
	}
	if(port){
		errors += thread_create(threads, port);
	}

	return errors;
}

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

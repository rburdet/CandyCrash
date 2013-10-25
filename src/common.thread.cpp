#include "common.thread.h"
#include <signal.h>

Thread::Thread() {}
Thread::~Thread() {}

void* Thread::runner(void* arg) {
	// Prevenir sigpipe, y manejar errores desde el write
	signal(SIGPIPE, SIG_IGN);
	return ((Thread*) arg)->run();
}

int Thread::start() {
	return pthread_create(&(this->me), NULL, Thread::runner, this);
}

int Thread::join() {
	return pthread_join(this->me, NULL);
}


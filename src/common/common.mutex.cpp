#include "common.mutex.h"

Mutex::Mutex(){
	pthread_mutex_init(&(this->me), NULL);
}
Mutex::~Mutex(){
	pthread_mutex_destroy(&(this->me));
}

int Mutex::lock(){
	return pthread_mutex_lock(&(this->me));
}

int Mutex::unlock(){
	return pthread_mutex_unlock(&(this->me));
}

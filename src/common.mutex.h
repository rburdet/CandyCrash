#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__
#include <pthread.h>

class Mutex{
	private:
		pthread_mutex_t me;

	public:
		Mutex();
		~Mutex();

		int lock();
		int unlock();
};

#endif

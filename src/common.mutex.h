#ifndef __COMMON_MUTEX_H__
#define __COMMON_MUTEX_H__
#include <pthread.h>

/** Encapsulamiento del mutex.
 */
class Mutex{
	private:
		pthread_mutex_t me;

	public:
		Mutex();
		~Mutex();

		/** Lockea al mutex
		 */
		int lock();
		/** Desloquea al mutex
		 */
		int unlock();
};

#endif

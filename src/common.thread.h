#ifndef __COMMON_THREAD_H__
#define __COMMON_THREAD_H__
#include <pthread.h>
class Thread {
	private:
		pthread_t me;
		static void* runner(void* ptr);

	protected:
		virtual void* run() = 0;

	public:
		Thread();
		virtual ~Thread();

		int start();
		int join();
};

#endif

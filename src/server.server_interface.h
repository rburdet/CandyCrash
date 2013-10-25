#ifndef __SERVER_INTERFACE_H__
#define __SERVER_INTERFACE_H__
#include "common.thread.h"

class ServerInterface {
	public:
		virtual void removeClient(Thread* cli) = 0;
};

#endif

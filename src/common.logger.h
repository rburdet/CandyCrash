#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__

#include <string>
#include "common.mutex.h"

class Logger {
	protected:
		Logger();
		~Logger();
		Mutex mut;
		void print(const std::string& str);
		static Logger* me;

	public:
		static void init();
		static void destroy();
		static void log(const std::string& str);
		static void log(const char* str);
};

#endif

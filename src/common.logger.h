#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__

#include <string>
#include "common.mutex.h"

/** Clase singleton usada para logger.
 * Bloque mutex antes de escribir al stdout, se la debe iniciar con init y destruir con destroy.
 */
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
		/** Funciones usadas para logear.
		 */
		static void log(const std::string& str);
		static void log(const char* str);
};

#endif

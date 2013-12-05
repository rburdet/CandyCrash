#ifndef __COMMON_PATH_MANAGER_H__
#define __COMMON_PATH_MANAGER_H__

#include <string>
#include "common.mutex.h"

/** Clase singleton usada para manajar los paths.
 * 
 */
class PathManager {
	protected:
		PathManager();
		~PathManager();
		static std::string exePath;

	public:
		static void init();
		static void destroy();
		static int chdir();
		static std::string getExePath();
		/** Se pasa un path relativo al binario y devuelve un absoluto.
		 */
		static std::string getPath(const std::string& str);
		static std::string getPath(const char* str);
};

#endif

#ifndef __COMMON_USER_MANAGER_H__
#define __COMMON_USER_MANAGER_H__

#include <string>
#include <jsoncpp/json/json.h>
#include "common.mutex.h"

/** Clase estilo singleton encargada del manejo de toda la informacion de los usuarios.
 * Se la debe iniciar con init y destruir con destroy.
 */
class UserManager {
	protected:
		UserManager(const std::string &path);
		~UserManager();

		std::string path;
		Json::Value users;
		Mutex mut;
		void _get(const std::string & user, Json::Value& data);
		int _set(const Json::Value& data);

		static UserManager* me;

	public:
		static void init(const std::string &path);
		static void destroy();
		/** Obtiene la informacion de un usario.
		 * @param user[in]: nombre del usuario.
		 * @param data[out]: informacion del usuario
		 */
		static void get(const std::string & user, Json::Value& data);
		/** Setea y guarda la informacion (recordar que el nombre de usaurio esta dentro de la data.
		 * @param data[in]: informacion del usuario
		 */
		static int set(const Json::Value& data);
};

#endif

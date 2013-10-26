#ifndef __COMMON_USER_MANAGER_H__
#define __COMMON_USER_MANAGER_H__

#include <string>
#include <jsoncpp/json/json.h>
#include "common.mutex.h"

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
		static void get(const std::string & user, Json::Value& data);
		static int set(const Json::Value& data);
};

#endif

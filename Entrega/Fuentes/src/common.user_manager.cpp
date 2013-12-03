#include "common.user_manager.h"

#include <fstream>
#include <string>

using std::ifstream;
using std::ofstream;

UserManager::UserManager(const std::string &path) : path(path) {
	ifstream ifs(this->path.c_str(), std::ifstream::in);
	if(ifs.is_open()){
		Json::Reader reader;
		std::string str((std::istreambuf_iterator<char>(ifs)), 
				std::istreambuf_iterator<char>());
		reader.parse(str, this->users);
	}
	ifs.close();
}

UserManager::~UserManager(){}

UserManager* UserManager::me = NULL;

void UserManager::init(const std::string &path){
	if(!UserManager::me)
		UserManager::me = new UserManager(path);
}

void UserManager::destroy(){
	if(UserManager::me)
		delete UserManager::me;
}

void UserManager::get(const std::string & user, Json::Value& data){
	UserManager::me->_get(user, data);
}

void UserManager::_get(const std::string & user, Json::Value& data){
	this->mut.lock();
	data = this->users[user];
	this->mut.unlock();
}

int UserManager::set(const Json::Value& data){
	return UserManager::me->_set(data);
}

int UserManager::_set(const Json::Value& data){
	this->mut.lock();
	Json::Value user = data;
	if(user["user"].isNull())
		return 1;

	if(user["pass"].isNull())
		return 1;

	if(user["nivel"].isNull())
		user["nivel"] = Json::Value(1);

	users[user["user"].asString()] = data;

	Json::StyledWriter writer;
	std::string outputConfig = writer.write(users);

	ofstream f(this->path.c_str(), std::ofstream::trunc);
	f << outputConfig;

	f.close();

	this->mut.unlock();

	return 0;
}

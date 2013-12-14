#include "common.logger.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

Logger::Logger(){}
Logger::~Logger(){}

Logger* Logger::me = NULL;
void Logger::init(){
	if(!Logger::me)
		Logger::me = new Logger();
}

void Logger::destroy(){
	if(Logger::me)
		delete Logger::me;
}

void Logger::print(const string &str){
	this->mut.lock();
	cout << str << endl;
	this->mut.unlock();
}

void Logger::log(const string &str){
	Logger::me->print(str);
}

void Logger::log(const char * str){
	string s(str);
	Logger::me->print(s);
}

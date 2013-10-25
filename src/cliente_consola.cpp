#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "cliente.socket_connect.h"

using std::string;
using std::stringstream;
using std::istream;
using std::ifstream;
using std::cout;
using std::cin;
using std::getline;
using std::endl;

int get_port(const string &ipport, string &port);

int main(int argc, char*argv[]){
	Json::Value data;
	if(argc != 2)
		return 1;

	TCPSocketConnect sock;
	string ip_port = argv[1];
	string port;
	string data_length;
	if(get_port(ip_port, port))
		return 1;

	if(sock.connect(ip_port))
		return 1;

	string user("usuario");
	string pass("password");
	if(sock.read(data, pass, false))
		return 2;

	Json::StaticString def("default");
	Json::Value msj = data.get("msj", def);

	//if(msj.compare("PUERTO "+port+" Aceptado. Recibiendo datos..."))
	//	return 2;

	cout << "[SERVIDOR] " << msj.asString() << endl;

	//msj.clear();

	Json::Value sendData;
	sendData["user"] = user;
	sendData["pass"] = pass;

	if(sock.write(sendData, pass))
		return 3;

	if(sock.read(data, pass))
		return 2;

	msj = data.get("msj", def);
	cout << "[SERVIDOR] " << msj.asString() << endl;

	//cout << "[SERVIDOR] " << msj << endl;

	return 0;
}

int get_port(const string &ipport, string &port){
	size_t found = ipport.find(':');
	if(found != std::string::npos){
		port = ipport.substr(found+1, ipport.length()-found);
		return 0;
	}

	return -1;
}

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "cliente.socket_connect.h"
#include "common.events.h"

/** Para usarlo se le pasa ip:puerto <archivo.json1> <archivo.json2>
 * Manda los archivos como mensajes de forma secuencial. El primero debe ser si o si un login!.
 * Recordar que la clave para cifrado la pide por stdin y no toma la del primer archivo
 */
using std::string;
using std::stringstream;
using std::istream;
using std::ifstream;
using std::cout;
using std::cin;
using std::getline;
using std::endl;
using Json::Value;
using Json::Reader;
using Json::StyledWriter;

int read_file(const char* path, string &str);
int get_port(const string &ipport, string &port);
int print_json(const Value& val);

int main(int argc, char*argv[]){
	if(argc < 3)
		return 1;

	TCPSocketConnect sock;
	string ip_port = argv[1];
	string port;
	string data_length;
	if(get_port(ip_port, port))
		return 1;

	if(sock.connect(ip_port))
		return 1;

	Value data;
	string pass;
	cout << "ingrese clave usada para cifrar" << endl;
	cin >> pass;
	cout << "Se cifrara con la siguiente clave: '" << pass << "'" << endl;

	if(sock.read(data, pass, false))
		return 2;

	print_json(data);

	for(int i=2; i < argc; i++){
		string msj;
		Value send;
		if(read_file(argv[i], msj)){
			cout << "error leyendo '" << argv[i] << "'" << endl;
			continue;
		}

		Reader reader;
		if(! reader.parse(msj, send)){
			cout << "error parseando '" << argv[i] << "'" << endl;
			continue;
		}

		if(sock.write(send, pass)){
			cout << "error mandando '" << argv[i] << "'" << endl;
			continue;
		}

		if(sock.read(data, pass)){
			cout << "error leyendo rta socket '" << argv[i] << "'" << endl;
			continue;
		}

		print_json(data);

	}

	return 0;
}

void read_all(istream& in, string &str){
	stringstream ss;

	char c = in.get();
	while(in.good()){
		ss << c;
		c = in.get();
	}

	str = ss.str();
}

int read_file(const char* path, string &str){
	ifstream ifs(path);
	if(!ifs.is_open())
		return -1;

	read_all(ifs, str);

	ifs.close();

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

int print_json(const Value& val){
	StyledWriter writer;
	string output = writer.write(val);
	cout << output << endl;

	return 0;
}

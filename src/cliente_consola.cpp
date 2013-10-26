#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "cliente.socket_connect.h"
#include "common.events.h"

/** Para usarlo se le pasa ip:puerto <archivo.json1> <archivo.json2>
 * Manda los archivos de manera secuencial.
 * La primera linea del archivo json tiene qe tener la clave qe se usara para firmar (SI O SI TIENE QE ESTAR ESTA CLAVE).
 * Si se pasa "wait" de archivo, el programa esperara hasta que se toque enter cuando llego a ese archivo.
 * Al finalizar la lista de archivos, el programa se qeda leyendo del stdin por mas archivos
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
using std::getline;
using Json::Value;
using Json::Reader;
using Json::StyledWriter;

int read_file(const char* path, string &pass, string &str);
int get_port(const string &ipport, string &port);
void send_json(TCPSocketConnect & sock, const string& path);
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

	if(sock.read(data, string(""), false))
		return 2;

	print_json(data);

	for(int i=2; i < argc; i++){
		string path = argv[i];
		if(path == "wait"){
			getline(cin, path);
			continue;
		}

		send_json(sock, path);
	}

	while(1){
		string path;
		getline(cin, path);
		if(path == "")
			break;
		send_json(sock, path);
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

int read_file(const char* path, string &pass, string &str){
	ifstream ifs(path);
	if(!ifs.is_open())
		return -1;

	getline(ifs, pass);

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

void send_json(TCPSocketConnect & sock, const string& path){
	string pass;
	string msj;
	Value send;
	Value data;

	cout << " == Archivo: '" << path << "' ==" << endl;
	if(read_file(path.c_str(), pass, msj)){
		cout << "\t[ERROR] leyendo" << endl;
		return;
	}

	cout << "=> Clave: '" << pass << "'" << endl;

	Reader reader;
	if(! reader.parse(msj, send)){
		cout << "\t[ERROR] parseando" << endl;
		return;
	}

	if(sock.write(send, pass)){
		cout << "\t[ERROR] mandando" << endl;
		return;
	}

	int ret = sock.read(data, pass);
	if(ret == 2){
		cout << "\t[ERROR] parseando json rta" << endl;
		return;
	}else if(ret == 1){
		cout << "\t[ERROR]  firma" << endl;
	}else if(ret < 0 || ret > 0){
		cout << "\t[ERROR] leyendo de socket ret:'" << ret << "'" << endl;
		return;
	}

	print_json(data);
}

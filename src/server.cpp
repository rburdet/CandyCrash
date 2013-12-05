#include <iostream>
#include <string>

#include "common.logger.h"
#include "common.user_manager.h"
#include "server.server.h"
#include "common.path_manager.h"

using std::cin;
using std::string;
using std::cout;
using std::endl;

/** str2number: transforma str a numero
 * @param [in] str puerto en string
 * @return numero
 */
int str2number(char* str);

/** Main de server.
 * Recibe como unico argumento de argv: puerto de escucha.
 * Se puede cerrar haciendo click en q.
 * @param [in] argc cantidad de argumentos (0 es el nombre del programa)
 * @param [in] argv parametros.
 * @return codigo de error.
 */
int main(int argc, char* argv[]){
	if(argc != 2){
		cout << "Uso: " << argv[0] << " <puerto>" << endl;
		return 1;
	}

	int port = str2number(argv[1]);

	if(port == 0){
		cout << "Puerto ingresado invalido" << endl;
		return 1;
	}

	cout << "Presione 'q' para cerrar" << endl;
	Logger::init();
	PathManager::init();
	PathManager::chdir();
	UserManager::init(string("./user_data.json"));

	Server server(port);

	server.start();

	while(getchar() != 'q'){
		Logger::log("Presione 'q' para cerrar");
	}

	server.end();

	Logger::destroy();
	UserManager::destroy();

	return 0;
}

int str2number(char* str){
	int port = 0;
	char c;

	for(int i=0; (c = str[i]); i++){
		switch(c){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				port = port * 10 + c - '0';
				break;

			default:
				break;
		}
	}

	return port;
}

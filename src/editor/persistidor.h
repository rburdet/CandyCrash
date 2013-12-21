#ifndef PERSISTIDOR_H
#define PERSISTIDOR_H

#include <fstream>
#include <stdlib.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

#define DEFAULT_MODE S_IRWXU | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH
#define DEFAULTDIR "../share/candycrash/Mapas/"

/** Clase encargada de volcar la informacion de los mapas a un archivo
 */

class Persistidor{
	public:
		/** Persiste el mapa.
		 * @param nivel: Nivel a persistir.
		 * @param nombre: Nombre del mapa a persistir.
		 */
		static void persistir(Json::Value& nivel,const std::string& nombre){
			//mkdir(DEFAULTDIR, DEFAULT_MODE);
			std::string aux = DEFAULTDIR + nombre + ".map";
			std::ofstream ofs(aux.c_str());
			if (ofs){
				ofs << nivel;
				ofs.close();
			}else{
				std::cerr<<"No se pudo crear el archivo" << std::endl;
				exit(1);
			}
		}
};

#endif 

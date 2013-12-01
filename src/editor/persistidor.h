#ifndef PERSISTIDOR_H
#define PERSISTIDOR_H

#include <fstream>
#include <stdlib.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

#define DEFAULT_MODE S_IRWXU | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH
#define DEFAULTDIR "../../Mapas"

class Persistidor{
	public:

		static void persistir(Json::Value& nivel,const std::string& nombre){
			mkdir(DEFAULTDIR, DEFAULT_MODE);
			std::string aux = "../../Mapas/" + nombre + ".map";
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

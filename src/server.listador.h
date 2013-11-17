#ifndef LISTADOR_H
#define LISTADOR_H

#include <jsoncpp/json/json.h>
#include "dirent.h"
#include <sstream>
#include <string>
#include <fstream>
#include "common.logger.h"

#define MAPA_PATH "../Mapas/"
#define MAPA_EXT ".map"

/** Clase que se encarga de proveer metodos para traer informacion acerca de los mapas.
 */
class Listador{
	public:
		/** Devuelve un objeto (hash) con: { nombre de archivo : nivel del mapa, }
		 */
		static Json::Value listar(){
			DIR* dir;
			struct dirent * ent;
			Json::Value file;
			if ((dir = opendir(MAPA_PATH)) != NULL ) {
				while (( ent = readdir(dir)) != NULL){
					std::string fname = std::string(ent->d_name);
					if(fname.find(MAPA_EXT) != std::string::npos){
						std::string fileName(ent->d_name);
						file[fileName] = getNivel(fileName);
					}
				}
				closedir(dir);
			}else{
				Logger::log(" No se pudo abrir el directorio de mapas! ");
			}

			return file;
		}

		//Si el nivel es -1 es xq no se pudo abrir, esto no deberia pasar.
		static int getNivel(std::string& fileName){
			int nivel = -1;
			Json::Value mapa;

			if(Listador::getMapa(fileName, mapa) == 0){
				Json::Value::Members keys = mapa.getMemberNames();
				if(keys.size()){
					std::string nMapa = keys[0];

					std::stringstream ss;
					ss << mapa[nMapa]["nivel"];
					ss >> nivel;
				}
			}

			return nivel;
		}

		// Devuelve 0 si salio bien, o algun numero si error (ponele qe no exista el mapa)
		static int getMapa(std::string& fileName, Json::Value& mapa){
			int ret = -1;
			std::string auxStr = MAPA_PATH + fileName;
			std::ifstream ifs;
			ifs.open(auxStr.c_str());
			if (ifs.is_open()){
				std::stringstream ss;
				Json::Reader reader;
				std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
				reader.parse(str, mapa);
				ret = 0;
			}
			ifs.close();
			return ret;
		}
};
#endif 

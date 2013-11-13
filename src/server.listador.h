#ifndef LISTADOR_H
#define LISTADOR_H

#include <jsoncpp/json/json.h>
#include "dirent.h"
#include <sstream>
#include <string>
#include <fstream>

class Listador{
	public:
		static Json::Value listar();
		static int getNivel(char* fileName);
};

//Si el nivel es -1 es xq no se pudo abrir, esto no deberia pasar.
int Listador::getNivel(char* fileName){
	int nivel=-1;
	std::ifstream ifs(fileName);
	if (ifs.is_open()){
		std::stringstream ss;
		Json::Value mapa;
		Json::Reader reader;
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		reader.parse(str, mapa);
		std::string nombre = std::string(fileName);
		std::string level = nombre.substr(0,nombre.find(".map"));
		ss<<mapa[level]["nivel"];
		ss>>nivel;
		ifs.close();
	}
	return nivel;
}


Json::Value Listador::listar(){
	DIR* dir;
	struct dirent * ent;
	Json::Value file;
	if ((dir = opendir("../Mapas/")) != NULL ) {
		while (( ent = readdir(dir)) != NULL){
			std::string fname = std::string(ent->d_name);
			if (fname.find(".map") != std::string::npos){
				file[ent->d_name] = getNivel(ent->d_name);
			}
		}
		closedir(dir);
	}else{
		std::cerr << " No se pudo abrir ! " << std::endl;
	}
	return file;
}

#endif 

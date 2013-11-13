#ifndef CREADOR_TABLERO_H
#define CREADOR_TABLERO_H

#include <vector>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <fstream>
#include <jsoncpp/json/json.h>

#define MAXELEMENTOS 16

class CreadorTablero{
	public:
		CreadorTablero(std::string fileName);
		~CreadorTablero(){}
		Json::Value getTablero();
		static int seeder;
	private:
		Json::Value tablero;
		Json::Value tableroFinal;
		void generar(std::string nombreNivel);
		void efectivizarCelda(Json::Value& celda);
		void getMax(double* arr,double& max, int& pos);
};

#endif

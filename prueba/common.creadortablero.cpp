#include "common.creadortablero.h"

CreadorTablero::CreadorTablero(std::string fileName){
	std::ifstream ifs(fileName.c_str());
	if (ifs.is_open()){
		Json::Reader reader;
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		reader.parse(str, this->tablero);
		this->tableroFinal = this->tablero;
		std::string nombreNivel = fileName.substr(0,fileName.find(".map"));
		this->generar(nombreNivel);
		ifs.close();
	}
}

void CreadorTablero::generar(std::string nombreNivel){
	std::stringstream ss,sy,sx;
	int x,y;
	ss << this->tablero[nombreNivel]["DIM"]["X"] << std::endl;
	ss >> x ; 
	ss << this->tablero[nombreNivel]["DIM"]["Y"] << std::endl;
	ss >> y ;
	for ( int i = 0 ; i < x ; i++ ) {
		sx<<i;
		for ( int j = 0 ; j < y ; j++ ) {
			Json::Value celda;
			sy<<j;
			celda=this->tablero[nombreNivel]["celdas"][sx.str()][sy.str()]["probabilidades"];
			efectivizarCelda(celda);
			this->tableroFinal[nombreNivel]["celdas"][sx.str()][sy.str()].removeMember("probabilidades");
			this->tableroFinal[nombreNivel]["celdas"][sx.str()][sy.str()]["pieza"]=celda;
			sy.str("");
		}
		sx.str("");
	}
}

void CreadorTablero::efectivizarCelda(Json::Value& celda){
	srand(time(NULL));
	double auxArr[MAXELEMENTOS];
	double max;
	int total=0;
	int pos;
	std::stringstream ss; 
	if (celda.isArray()){
		int aux;
		for ( int i = 0 ; i < MAXELEMENTOS ; i++ ){
			ss << celda[i];
			ss >> aux;
			auxArr[i] = aux;
			total += aux ;
			ss.str("");
		}
		if (total){
			for ( int i = 0 ; i < MAXELEMENTOS ; i++ ){
				std::cout << auxArr[i]<< " " ;
			}
			std::cout << std::endl;
			getMax(auxArr,max,pos);
			celda = pos;
		}else{
			int* seeder = new int;
			srand(time(NULL) * (int)seeder);
			aux = (int)(rand() % MAXELEMENTOS);
			delete seeder;
			celda=aux;
		}
	}
}


void CreadorTablero::getMax(double* auxArr,double& max, int& pos){
	max = auxArr[0];
	pos = 0;
	for ( int i = 0 ; i < MAXELEMENTOS-1 ; i++ ) {
		if ( auxArr[i] > auxArr[i+1] ){
			max = auxArr[i];
			pos = i;
		}
	}
}

Json::Value CreadorTablero::getTablero(){
	return this->tableroFinal;
}

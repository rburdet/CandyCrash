#include "server.tablero.h"
#include "server.listador.h"
#include "common.logger.h"
#include "common.caramelos.h"

#include <sstream>
#include <cstdlib>
#include <time.h>

#define MAXELEMENTOS 16
// Puntos que se adquieren por cada elemento en un evento con una star
#define PUNTOS_ELEM_STAR 40

using std::string;
using Json::Value;
using std::stringstream;

Tablero::Tablero(string path){
	Json::Value data;
	Listador::getMapa(path, data);
	if(Listador::getMapa(path, data) == 0){
		Json::Value::Members keys = data.getMemberNames();
		if(keys.size()){
			std::string nMapa = keys[0];

			std::stringstream ss;
			this->mapa = data[nMapa];
			this->probabilidades = this->mapa["columnas"];
			this->generar();
		}
	}else{
		Logger::log("Error creando mapa '"+path+"' Inexistente!");
	}
}

void Tablero::generar(){
	stringstream ss,sy,sx;

	ss << this->mapa["DIM"]["X"] << std::endl;
	ss >> this->dim_x ; 
	ss << this->mapa["DIM"]["Y"] << std::endl;
	ss >> this->dim_y ;

	for ( int i = 0 ; i < this->dim_x ; i++ ) {
		sx<<i;
		for ( int j = 0 ; j < this->dim_y ; j++ ) {
			Json::Value celda;
			sy<<j;
			celda = this->mapa["celdas"][sx.str()][sy.str()]["probabilidades"];
			this->efectivizarCelda(celda);
			this->tablero[sx.str()][sy.str()] = celda;
			sy.str("");
		}
		sx.str("");
	}
}

void Tablero::efectivizarCelda(Json::Value& celda){
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
			auxArr[i] = aux*rand();
			total += aux ;
			ss.str("");
		}
		if (total){
			getMax(auxArr,max,pos);
			celda = pos;
		}else{
			int* seeder = new int;
			srand(time(NULL) * (int)seeder);
			aux = (int)(rand() % MAXELEMENTOS);
			delete seeder;
			celda = aux;
		}
	}
}

void Tablero::getMax(double* auxArr,double& max, int& pos){
	max = auxArr[0];
	pos = 0;
	for ( int i = 0 ; i < MAXELEMENTOS-1 ; i++ ) {
		if ( auxArr[i] > auxArr[i+1] ){
			max = auxArr[i];
			pos = i;
		}
	}
}

int Tablero::movimiento(int x, int y, CaramelosMovimientos mov, Json::Value& movimientos){
	Json::Value movs;
	if(! this->movimientoValido(x, y, mov))
		return 0;

	int xf, yf;

	int puntos = 0;

	this->calcularCoordenadas(x, y, mov, xf, yf);

	int carameloMovido = this->tablero[this->num2str(y)][this->num2str(x)].asInt();
	int carameloMovido2 = this->tablero[this->num2str(yf)][this->num2str(xf)].asInt();

	Json::Value aux = this->tablero[this->num2str(y)][this->num2str(x)];
	this->tablero[this->num2str(y)][this->num2str(x)] = this->tablero[this->num2str(yf)][this->num2str(xf)];
	this->tablero[this->num2str(yf)][this->num2str(xf)] = aux;

	movs.append(this->newMov(x, y, mov));
	movs.append(this->newMov(xf, yf, this->movInverso(mov)));

	if(carameloMovido == STAR || carameloMovido2 == STAR){
		puntos += this->doStar((Caramelos) carameloMovido, xf, yf, (Caramelos) carameloMovido2, x, y, movs);
		this->rellenarTablero(movs);
	}

	while(this->hayPatrones(movs, puntos))
		this->rellenarTablero(movs);

	movimientos = movs;

	return puntos;
}

bool Tablero::movimientoValido(int x, int y, CaramelosMovimientos mov){
	int x2 = x, y2 = y;
	this->calcularCoordenadas(x, y, mov, x2, y2);

	if(x == x2 && y == y2)
		return false;

	if(x2 < 0 || y2 < 0 || x2 >= this->dim_x || y2 >= this->dim_y)
		return false;

	int carameloMovido = this->tablero[this->num2str(y)][this->num2str(x)].asInt();
	int carameloMovido2 = this->tablero[this->num2str(y2)][this->num2str(x2)].asInt();

	if(carameloMovido == STAR || carameloMovido2 == STAR)
		return true;

	// Cantidad de veces que esta el mismo color en y (columna), x (fila)
	int y_color, x_color, y_color2, x_color2;
	for(int i=-2; i < 3; i++){
		int new_x = x + i,
			new_y = y + i,
			new_x2 = x2 + i,
			new_y2 = y2 + i;

		if( i == 0 ){
			new_x = x2;
			new_y = y2;
			new_x2 = x;
			new_y2 = y;
		}

		if(new_x >= 0 && new_x < this->dim_x && this->esMismoColor((Caramelos) carameloMovido2, (Caramelos) this->tablero[this->num2str(y)][this->num2str(new_x)].asInt()))
			x_color++;
		else
			x_color = 0;

		if(new_y >= 0 && new_y < this->dim_y && this->esMismoColor((Caramelos) carameloMovido2, (Caramelos) this->tablero[this->num2str(new_y)][this->num2str(x)].asInt()))
			y_color++;
		else
			y_color = 0;

		if(new_x2 >= 0 && new_x2 < this->dim_x && this->esMismoColor((Caramelos) carameloMovido, (Caramelos) this->tablero[this->num2str(y2)][this->num2str(new_x2)].asInt()))
			x_color2++;
		else
			x_color2 = 0;

		if(new_y2 >= 0 && new_y2 < this->dim_y && this->esMismoColor((Caramelos) carameloMovido, (Caramelos) this->tablero[this->num2str(new_y2)][this->num2str(x2)].asInt()))
			y_color2++;
		else
			y_color2 = 0;

		if(x_color >= 3 || y_color >= 3 || x_color2 >= 3 || y_color2 >= 3)
			return true;
	}


	return false;
}

std::string Tablero::num2str(int n){
	stringstream ss;
	ss << n;
	string ret(ss.str());
	return ret;
}

bool Tablero::esMismoColor(Caramelos car, Caramelos car2){
	switch(car){
		case RED_BUTTON:
			if(car2 == RED_BUTTON || car2 == RED_HORBAR || car2 == RED_VERBAR)
				return true;
			break;

		case BLUE_BUTTON:
			if(car2 == BLUE_BUTTON || car2 == BLUE_HORBAR || car2 == BLUE_VERBAR)
				return true;
			break;

		case GREEN_BUTTON:
			if(car2 == GREEN_BUTTON || car2 == GREEN_HORBAR || car2 == GREEN_VERBAR)
				return true;
			break;

		case PURPLE_BUTTON:
			if(car2 == PURPLE_BUTTON || car2 == PURPLE_HORBAR || car2 == PURPLE_VERBAR)
				return true;
			break;

		case YELLOW_BUTTON:
			if(car2 == YELLOW_BUTTON || car2 == YELLOW_HORBAR || car2 == YELLOW_VERBAR)
				return true;
			break;

		case RED_HORBAR:
			if(car2 == RED_BUTTON || car2 == RED_HORBAR || car2 == RED_VERBAR)
				return true;
			break;

		case BLUE_HORBAR:
			if(car2 == BLUE_BUTTON || car2 == BLUE_HORBAR || car2 == BLUE_VERBAR)
				return true;
			break;

		case GREEN_HORBAR:
			if(car2 == GREEN_BUTTON || car2 == GREEN_HORBAR || car2 == GREEN_VERBAR)
				return true;
			break;

		case PURPLE_HORBAR:
			if(car2 == PURPLE_BUTTON || car2 == PURPLE_HORBAR || car2 == PURPLE_VERBAR)
				return true;
			break;

		case YELLOW_HORBAR:
			if(car2 == YELLOW_BUTTON || car2 == YELLOW_HORBAR || car2 == YELLOW_VERBAR)
				return true;
			break;

		case RED_VERBAR:
			if(car2 == RED_BUTTON || car2 == RED_HORBAR || car2 == RED_VERBAR)
				return true;
			break;

		case BLUE_VERBAR:
			if(car2 == BLUE_BUTTON || car2 == BLUE_HORBAR || car2 == BLUE_VERBAR)
				return true;
			break;

		case GREEN_VERBAR:
			if(car2 == GREEN_BUTTON || car2 == GREEN_HORBAR || car2 == GREEN_VERBAR)
				return true;
			break;

		case PURPLE_VERBAR:
			if(car2 == PURPLE_BUTTON || car2 == PURPLE_HORBAR || car2 == PURPLE_VERBAR)
				return true;
			break;

		case YELLOW_VERBAR:
			if(car2 == YELLOW_BUTTON || car2 == YELLOW_HORBAR || car2 == YELLOW_VERBAR)
				return true;
			break;
		default:
			false;
	}

	return false;
}

void Tablero::calcularCoordenadas(int x, int y, CaramelosMovimientos mov, int&xf, int &yf){
	xf = x;
	yf = y;
	switch(mov){
		case CARAMELO_MOV_ARRIBA:
			yf--;
			break;

		case CARAMELO_MOV_DERECHA:
			xf++;
			break;

		case CARAMELO_MOV_ABAJO:
			yf++;
			break;

		case CARAMELO_MOV_IZQ:
			xf--;
			break;

		default:
			break;
	}
}


int Tablero::doStar(Caramelos carameloMovido, int x, int y, Caramelos carameloMovido2, int xf, int yf, Json::Value& movimientos){
	int puntos = 0;

	if(carameloMovido == STAR && carameloMovido2 == STAR){
		puntos += this->dim_y * this->dim_x * PUNTOS_ELEM_STAR;
		for(int i=0 ; i < this->dim_y ; i++){
			for(int j=0 ; j < this->dim_x ; j++){
				this->tablero[this->num2str(i)][this->num2str(j)] = RELLENAR;
				movimientos.append(this->newMov(j, i, CARAMELO_MOV_LIMBO));
			}
		}
	}else{
		Caramelos elem_no = carameloMovido;
		int x_no = x, y_no = y;

		// Sumo los puntos de la estrella
		puntos += 40;

		if(carameloMovido == STAR){
			elem_no = carameloMovido2;
			x_no = xf;
			y_no = yf;
			this->tablero[this->num2str(y)][this->num2str(x)] = RELLENAR;
			movimientos.append(this->newMov(x, y, CARAMELO_MOV_LIMBO));
		}else{
			this->tablero[this->num2str(yf)][this->num2str(xf)] = RELLENAR;
			movimientos.append(this->newMov(xf, yf, CARAMELO_MOV_LIMBO));
		}

		switch(elem_no){
			case RED_HORBAR:
			case BLUE_HORBAR:
			case GREEN_HORBAR:
			case PURPLE_HORBAR:
			case YELLOW_HORBAR:
			case RED_VERBAR:
			case BLUE_VERBAR:
			case GREEN_VERBAR:
			case PURPLE_VERBAR:
			case YELLOW_VERBAR:
				for(int i=0; i < this->dim_y; i++){
					if(i == y_no)
						continue;
					this->tablero[this->num2str(i)][this->num2str(x_no)] = RELLENAR;
					puntos += PUNTOS_ELEM_STAR;
				}
				for(int i=0; i < this->dim_x; i++){
					if(i == x_no)
						continue;
					this->tablero[this->num2str(y_no)][this->num2str(i)] = RELLENAR;
					puntos += PUNTOS_ELEM_STAR;
				}

			case RED_BUTTON:
			case BLUE_BUTTON:
			case GREEN_BUTTON:
			case PURPLE_BUTTON:
			case YELLOW_BUTTON:
				for(int i=0 ; i < this->dim_y ; i++){
					for(int j=0 ; j < this->dim_x ; j++){
						if(this->esMismoColor(elem_no, (Caramelos) this->tablero[this->num2str(i)][this->num2str(j)].asInt())){
							this->tablero[this->num2str(i)][this->num2str(j)] = RELLENAR;
							movimientos.append(this->newMov(j, i, CARAMELO_MOV_LIMBO));
							puntos += 40;
						}
					}
				}
				break;
			default:
				break;
		}
	}

	return puntos;
}

Json::Value Tablero::newMov(int x, int y, CaramelosMovimientos mov){
	Json::Value val;
	val["x"] = x;
	val["y"] = y;
	val["mov"] = mov;
	return val;
}

CaramelosMovimientos Tablero::movInverso(CaramelosMovimientos mov){
	switch(mov){
		case CARAMELO_MOV_ARRIBA:
			return CARAMELO_MOV_ABAJO;
			break;

		case CARAMELO_MOV_DERECHA:
			return CARAMELO_MOV_IZQ;
			break;

		case CARAMELO_MOV_ABAJO:
			return CARAMELO_MOV_ARRIBA;
			break;

		case CARAMELO_MOV_IZQ:
			return CARAMELO_MOV_DERECHA;
			break;

		default:
			break;
	}

	return mov;
}

void Tablero::rellenarTablero(Json::Value& movimientos){
	for(int y=this->dim_y-1; y >= 0; y--){
		for(int x=this->dim_x-1; x >= 0; x--){
			Caramelos car = (Caramelos) this->tablero[this->num2str(y)][this->num2str(x)].asInt();

			if(car != RELLENAR)
				continue;

			if(y == 0){
				Json::Value celda = this->probabilidades[this->num2str(x)];
				this->efectivizarCelda(celda);
				this->tablero[this->num2str(y)][this->num2str(x)] = celda;
				movimientos.append(this->newMov(x, y, CARAMELO_MOV_NEW));
			}else{
				int y_rem = y--;
				car = (Caramelos) this->tablero[this->num2str(y_rem)][this->num2str(x)].asInt();
				if(car == HUECO || car == RELLENAR){
					while( y_rem > 0 && (car == HUECO || car == RELLENAR))
						car = (Caramelos) this->tablero[this->num2str(--y_rem)][this->num2str(x)].asInt();

					if(y_rem == 0 && (car == HUECO || car == RELLENAR)){
						Json::Value celda = this->probabilidades[this->num2str(x)];
						this->efectivizarCelda(celda);
						this->tablero[this->num2str(y)][this->num2str(x)] = celda;
						movimientos.append(this->newMov(x, y_rem, CARAMELO_MOV_NEW));
					}else{
						this->tablero[this->num2str(y)][this->num2str(x)] = car;
						this->tablero[this->num2str(y_rem)][this->num2str(x)] = RELLENAR;
					}

					for(int y_mov = y_rem; y_mov < y ; y_mov++)
						movimientos.append(this->newMov(x, y_mov, CARAMELO_MOV_ABAJO));

				}else{
					this->tablero[this->num2str(y)][this->num2str(x)] = car;
					this->tablero[this->num2str(y_rem)][this->num2str(x)] = RELLENAR;
					movimientos.append(this->newMov(x, y_rem, CARAMELO_MOV_ABAJO));
				}
			}
		}
	}
}

bool Tablero::hayPatrones(Json::Value & movimientos, int & puntos){
	return false;
}
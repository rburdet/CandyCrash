#include "server.tablero.h"
#include "server.listador.h"
#include "common.logger.h"
#include "common.caramelos.h"

#include <sstream>
#include <cstdlib>
#include <time.h>
#include <random>
#include <chrono>


#define MAXELEMENTOS 16
// Puntos que se adquieren por cada elemento en un evento con una star
#define PUNTOS_ELEM_STAR 40
#define PUNTOS_BUTTON_CINCO 30
#define PUNTOS_BUTTON_CUATRO 20
#define PUNTOS_BUTTON_TRES 10

using std::string;
using Json::Value;
using std::stringstream;

Tablero::Tablero(string path){
	Json::Value data;
	Listador::getMapa(path, data);
	if(Listador::getMapa(path, data) == 0){
		Json::Value::Members keys = data.getMemberNames();
		if(keys.size()){
			this->nMapa = keys[0];
			std::stringstream ss;
			this->mapa = data[this->nMapa];
			this->probabilidades = this->mapa["columnas"];
			this->imagenFondo = this->mapa["fondo"];
			this->pathFondo=imagenFondo.asString();
			this->generar();
			// Sacamos las combinaciones iniciales
			int puntos;
			Json::Value movs;
			// TODO: podria entrar en un loop infinito, si las probablidades de las fichas que se crean siempre dan combinaciones validas (el sistema de probabilidades, permite decir que siempre se cree una sola ficha)
			while(this->hayPatrones(movs, puntos))
				this->rellenarTablero(movs);
		}
	}else{
		Logger::log("Error creando mapa '"+path+"' Inexistente!");
	}
}
Tablero::~Tablero(){
}

void Tablero::generar(){
	stringstream ss,sy,sx;
	ss << this->mapa["DIM"]["X"] << std::endl;
	ss >> this->dim_x ; 
	this->alto = this->dim_x;
	ss << this->mapa["DIM"]["Y"] << std::endl;
	ss >> this->dim_y ;
	this->ancho =  this->dim_y;
	for ( int i = 0 ; i < this->alto ; i++ ) {
		sy<<i;
		for ( int j = 0 ; j < this->ancho ; j++ ) {
			Json::Value celda;
			sx<<j;
			celda = this->mapa["celdas"][sx.str()][sy.str()]["probabilidades"];
			this->efectivizarCelda(celda);
			this->tablero[sx.str()][sy.str()] = celda;
			sx.str("");
		}
		sy.str("");
	}
}

void Tablero::efectivizarCelda(Json::Value& celda){
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0,15);
	std::uniform_int_distribution<int> distribution2(0,99);
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
			auxArr[i] = aux*distribution2(generator);
			total += aux ;
			ss.str("");
		}
		if (total){
			getMax(auxArr,max,pos);
			celda = pos;
		}else{
			celda = distribution(generator);
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
	if (auxArr[MAXELEMENTOS-1] > auxArr[pos])
		pos = MAXELEMENTOS-1;
}

int Tablero::movimiento(int x, int y, CaramelosMovimientos mov, Json::Value& movimientos){
	Json::Value movs;
	// Compruebo que sea movimiento valido
	Logger::log("es valido el movimiento?");

	if(! this->movimientoValido(x, y, mov)){
		Logger::log("no es valido!");
		return 0;
	}

	Logger::log("si");

	int xf, yf;

	int puntos = 0;

	// Calculo las nuevas coordenadas nuevas del caramelo
	this->calcularCoordenadas(x, y, mov, xf, yf);

	// Obtengo el caramelo que se mueve
	int carameloMovido = this->tablero[this->num2str(x)][this->num2str(y)].asInt();
	// Obtengo el caramelo que esta donde se va a mover el otro
	int carameloMovido2 = this->tablero[this->num2str(xf)][this->num2str(yf)].asInt();

	// Pongo los caramelos en sus respectivas posiciones
	Json::Value aux = this->tablero[this->num2str(x)][this->num2str(y)];
	this->tablero[this->num2str(x)][this->num2str(y)] = this->tablero[this->num2str(xf)][this->num2str(yf)];
	this->tablero[this->num2str(xf)][this->num2str(yf)] = aux;

	movs.append(this->newMov(x, y, mov));
	movs.append(this->newMov(xf, yf, this->movInverso(mov)));

	if(carameloMovido == STAR || carameloMovido2 == STAR){ // Si uno de los caramelos movidos es STAR, efectuo estar.
		Logger::log("Movimiento star ");
		puntos += this->doStar((Caramelos) carameloMovido, xf, yf, (Caramelos) carameloMovido2, x, y, movs);
	}else{ // efectuo combinacion por cada caramelo
		Logger::log("Movimiento normal ");
		puntos += this->doCombinacion((Caramelos) carameloMovido, xf, yf, movs);
		puntos += this->doCombinacion((Caramelos) carameloMovido2, x, y, movs);
	}

	// Relleno el tablero de vuelta
	this->rellenarTablero(movs);

	// TODO: habria qe ver qe se multiplican por las combinaciones qe se generen por qe se caigan las fichas. El enunciado dice algo de eso.
	// TODO: podria entrar en un loop infinito, si las probablidades de las fichas que se crean siempre dan combinaciones validas (el sistema de probabilidades, permite decir que siempre se cree una sola ficha)
	while(this->hayPatrones(movs, puntos))
		this->rellenarTablero(movs);

	movimientos = movs;

	std::cout << "Puntos: " << puntos << std::endl;

	// meow meow meow meooooooooooow
	return puntos;
}

bool Tablero::hayCombinacion(bool todosButtons, int contador){
	if(todosButtons){
		if(contador >=3)
			return true;
	}else{
		if(contador >=4)
			return true;
	}
	return false;

}

bool Tablero::hayMovimiento(int x, int y, Caramelos car){
	int contador = 0;
	bool todosButtons = this->esButton(car);
	int x_m;
	for(x_m=x-4 ; x_m < this->dim_x && x_m < x + 4; x_m++){
		if(x_m < 0)
			continue;

		Caramelos esteCar = (Caramelos) this->tablero[this->num2str(x_m)][this->num2str(y)].asInt();
		if(this->esMismoColor(car, esteCar)){
			contador++;
			if(! this->esButton(esteCar) )
				todosButtons = false;
		}else{
			if(x_m >= x)
				if(this->hayCombinacion(todosButtons, contador))
					return true;

			contador = 0;
			todosButtons = this->esButton(car);
			if(x_m > x)
				break;
		}
	}

	if(this->hayCombinacion(todosButtons, contador))
		return true;

	contador = 0;
	todosButtons = this->esButton(car);

	int y_m;
	for(y_m=y-4 ; y_m < this->dim_y && y_m < y + 4; y_m++){
		if(y_m < 0)
			continue;

		Caramelos esteCar = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y_m)].asInt();
		if(this->esMismoColor(car, esteCar)){
			contador++;
			if(! this->esButton(esteCar) )
				todosButtons = false;
		}else{
			if(y_m >= y)
				if(this->hayCombinacion(todosButtons, contador))
					return true;
			contador = 0;
			todosButtons = this->esButton(car);

			if(y_m > y)
				break;
		}
	}

	if(this->hayCombinacion(todosButtons, contador))
		return true;

	return false;
}

bool Tablero::movimientoValido(int x, int y, CaramelosMovimientos mov){
	int x2 = x, y2 = y;
	this->calcularCoordenadas(x, y, mov, x2, y2);

	if(x == x2 && y == y2)
		return false;

	if(x2 < 0 || y2 < 0 || x2 >= this->dim_x || y2 >= this->dim_y)
		return false;

	int carameloMovido = this->tablero[this->num2str(x)][this->num2str(y)].asInt();
	int carameloMovido2 = this->tablero[this->num2str(x2)][this->num2str(y2)].asInt();

	// Si alguno de esos caramelos es un STAR, ya es un movimeinto valido
	if(carameloMovido == STAR || carameloMovido2 == STAR)
		return true;

	Json::Value aux = this->tablero[this->num2str(x)][this->num2str(y)];
	//Json::Value aux2 = this->tablero[this->num2str(y2)][this->num2str(x2)];
	this->tablero[this->num2str(x)][this->num2str(y)] = this->tablero[this->num2str(x2)][this->num2str(y2)];
	this->tablero[this->num2str(x2)][this->num2str(y2)] = aux;

	bool ret = false;
	if(this->hayMovimiento(x, y, (Caramelos) carameloMovido2))
		ret = true;

	if(this->hayMovimiento(x2, y2, (Caramelos) carameloMovido))
		ret = true;

	this->tablero[this->num2str(x)][this->num2str(y)] = carameloMovido;
	this->tablero[this->num2str(x2)][this->num2str(y2)] = carameloMovido2;

	return ret;
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
		for(int i=0 ; i < this->dim_y ; i++){ // X
			for(int j=0 ; j < this->dim_x ; j++){ //Y
				if(this->tablero[this->num2str(i)][this->num2str(j)] != HUECO){
					this->tablero[this->num2str(i)][this->num2str(j)] = RELLENAR;
					movimientos.append(this->newMov(i, j, CARAMELO_MOV_LIMBO));
				}
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
			if(this->tablero[this->num2str(x)][this->num2str(y)] != HUECO){
				this->tablero[this->num2str(x)][this->num2str(y)] = RELLENAR;
				movimientos.append(this->newMov(x, y, CARAMELO_MOV_LIMBO));
			}
		}else{
			if(this->tablero[this->num2str(xf)][this->num2str(yf)] != HUECO){
				this->tablero[this->num2str(xf)][this->num2str(yf)] = RELLENAR;
				movimientos.append(this->newMov(xf, yf, CARAMELO_MOV_LIMBO));
			}
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
					if(this->tablero[this->num2str(x_no)][this->num2str(i)] != HUECO){
						this->tablero[this->num2str(x_no)][this->num2str(i)] = RELLENAR;
						puntos += PUNTOS_ELEM_STAR;
						movimientos.append(this->newMov(x_no, i, CARAMELO_MOV_LIMBO));
					}
				}
				for(int i=0; i < this->dim_x; i++){
					if(i == x_no)
						continue;
					if(this->tablero[this->num2str(i)][this->num2str(y_no)] != HUECO){
						this->tablero[this->num2str(i)][this->num2str(y_no)] = RELLENAR;
						puntos += PUNTOS_ELEM_STAR;
						movimientos.append(this->newMov(i, y_no, CARAMELO_MOV_LIMBO));
					}
				}

			case RED_BUTTON:
			case BLUE_BUTTON:
			case GREEN_BUTTON:
			case PURPLE_BUTTON:
			case YELLOW_BUTTON:
				for(int i=0 ; i < this->dim_y ; i++){ // X
					for(int j=0 ; j < this->dim_x ; j++){ // Y
						if(this->esMismoColor(elem_no, (Caramelos) this->tablero[this->num2str(i)][this->num2str(j)].asInt())){
							if(this->tablero[this->num2str(i)][this->num2str(j)] != HUECO){
								this->tablero[this->num2str(i)][this->num2str(j)] = RELLENAR;
								movimientos.append(this->newMov(i, j, CARAMELO_MOV_LIMBO));
								puntos += 40;
							}
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

Json::Value Tablero::newMov(int x, int y, CaramelosMovimientos mov, Caramelos car){
	Json::Value val = this->newMov(x, y, mov);
	val["car"] = car;
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
		for(int x=0; x < this->dim_x; x++){
			Caramelos car = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y)].asInt();

			if(car != RELLENAR)
				continue;

			if(y == 0){
				Json::Value celda = this->probabilidades[this->num2str(x)];
				this->efectivizarCelda(celda);
				this->tablero[this->num2str(x)][this->num2str(y)] = celda;
				movimientos.append(this->newMov(x, y, CARAMELO_MOV_NEW, (Caramelos) celda.asInt()));
			}else{
				int y_rem = y-1;
				car = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y_rem)].asInt();
				if(car == HUECO || car == RELLENAR){
					while( y_rem > 0 && (car == HUECO || car == RELLENAR))
						car = (Caramelos) this->tablero[this->num2str(x)][this->num2str(--y_rem)].asInt();

					if(y_rem == 0 && (car == HUECO || car == RELLENAR)){
						Json::Value celda = this->probabilidades[this->num2str(x)];
						this->efectivizarCelda(celda);
						this->tablero[this->num2str(x)][this->num2str(y)] = celda;
						movimientos.append(this->newMov(x, y_rem, CARAMELO_MOV_NEW, (Caramelos) celda.asInt()));
					}else{
						this->tablero[this->num2str(x)][this->num2str(y)] = car;
						this->tablero[this->num2str(x)][this->num2str(y_rem)] = RELLENAR;
					}

					for(int y_mov = y_rem; y_mov < y ; y_mov++)
						movimientos.append(this->newMov(x, y_mov, CARAMELO_MOV_ABAJO));

				}else{
					this->tablero[this->num2str(x)][this->num2str(y)] = car;
					this->tablero[this->num2str(x)][this->num2str(y_rem)] = RELLENAR;
					movimientos.append(this->newMov(x, y_rem, CARAMELO_MOV_ABAJO));
				}
			}
		}
	}
}

bool Tablero::hayPatrones(Json::Value & movimientos, int & puntos){
	// Para encontrar patrones, vamos a recorrer por filas y columnas viendo si hay fichas del mismo color
	// Cuento fichas del mismo color y tengo un bool para saber si son solo buttons o hay fichas mezcladas.
	// Cuando me topo con una ficha que no es del mismo color, me fijo en que qedo el contador y el booleano. Dependiendo de esos valores efectuo acciones.
	// TODO: como primero se fija por las filas y dsp por las columnas, tienen prioridad las filas.
	// El enunciado no habla nada de que hacer si se superponen dos combinaciones, siempre se activaran las filas primeros, se podria cambiar este comportamiento, haciendo un poco mas complicada la cosa,,
	bool huboMovimiento = false;
	
	// Itero por todas las filas
	for(int y=0; y < this->dim_y; y++){
		//Traigo primer caramelo
		Caramelos car = (Caramelos) this->tablero["0"][this->num2str(y)].asInt();
		int contador = 1; //Contador de caramelos con el mismo color. 1, por que el caramelo es igual a si mismo
		bool todosButtons = this->esButton(car); // Flag para saber si son todos buttons
		int x;
		for(x=1; x < this->dim_x; x++){ // Itero por una fila
			// Obtengo el caramelo de la posicion
			Caramelos esteCar = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y)].asInt();
			// Si es del mismo color que el caramelo que tenia, sumo contador
			if(this->esMismoColor(car, esteCar)){
				contador++;
				if(! this->esButton(esteCar) )
					todosButtons = false;
			}else{
				// Sino, llamo a activarCombinacionFila, que hace la magia (ver comentarios de eso)
				if(this->activarCombinacionFila(x, y, todosButtons, contador, movimientos, puntos))
					huboMovimiento = true;

				car = esteCar;
				contador = 1;
				todosButtons = this->esButton(car);
			}
		}

		// Al final vuelvo a llamar activarCombinacionFila
		if(this->activarCombinacionFila(x, y, todosButtons, contador, movimientos, puntos))
			huboMovimiento = true;
	}

	// Itero por todas las columnas (es analogo al anterior)
	for(int x=0; x < this->dim_x; x++){
		Caramelos car = (Caramelos) this->tablero[this->num2str(x)]["0"].asInt();
		int contador = 1;
		bool todosButtons = this->esButton(car);
		int y;
		for(y=1; y < this->dim_y; y++){
			Caramelos esteCar = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y)].asInt();
			if(this->esMismoColor(car, esteCar)){
				contador++;
				if(! this->esButton(esteCar) )
					todosButtons = false;
			}else{
				if(this->activarCombinacionColumna(x, y, todosButtons, contador, movimientos, puntos))
					huboMovimiento = true;

				car = esteCar;
				contador = 1;
				todosButtons = this->esButton(car);
			}
		}

		if(this->activarCombinacionColumna(x, y, todosButtons, contador, movimientos, puntos))
			huboMovimiento = true;
	}
	
	return huboMovimiento;
}

bool Tablero::esButton(Caramelos car){
	switch(car){
		case RED_BUTTON:
		case BLUE_BUTTON:
		case GREEN_BUTTON:
		case PURPLE_BUTTON:
		case YELLOW_BUTTON:
			return true;
			break;

		default:
			break;
	}

	return false;
}

void Tablero::dispararFila(int y, Json::Value& movimientos, int puntosx, int& puntos){
	// Recorro toda la fila sacando los caramelos, sumando puntos y agragando el movimiento a movimientos
	for(int x_m =0; x_m < this->dim_x; x_m++){
		Caramelos car = (Caramelos) this->tablero[this->num2str(x_m)][this->num2str(y)].asInt();
		switch(car){
			case RED_BUTTON:
			case BLUE_BUTTON:
			case GREEN_BUTTON:
			case PURPLE_BUTTON:
			case YELLOW_BUTTON:
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
			case STAR:
				if(this->tablero[this->num2str(x_m)][this->num2str(y)] != HUECO){
					puntos += puntosx;
					this->tablero[this->num2str(x_m)][this->num2str(y)] = RELLENAR;
					movimientos.append(this->newMov(x_m, y, CARAMELO_MOV_LIMBO));
				}
				break;

			default:
				break;
		}
	}
}

void Tablero::dispararColumna(int x, Json::Value& movimientos, int puntosx, int& puntos){
	for(int y_m =0; y_m < this->dim_y; y_m++){
		Caramelos car = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y_m)].asInt();
		switch(car){
			case RED_BUTTON:
			case BLUE_BUTTON:
			case GREEN_BUTTON:
			case PURPLE_BUTTON:
			case YELLOW_BUTTON:
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
			case STAR:
				if(this->tablero[this->num2str(x)][this->num2str(y_m)] != HUECO){
					puntos += puntosx;
					this->tablero[this->num2str(x)][this->num2str(y_m)] = RELLENAR;
					movimientos.append(this->newMov(x, y_m, CARAMELO_MOV_LIMBO));
				}
				break;

			default:
				break;
		}
	}
}

bool Tablero::activarCombinacionFila(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos){
	return this->activarCombinacionFila(x, y, todosButtons, contador, movimientos, puntos, x-contador/2, y);
}

bool Tablero::activarCombinacionFila(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos, int x_mov, int y_mov){
	bool huboMovimiento = false;
	Caramelos carameloEspecial = RELLENAR;

	if(todosButtons){ // Si son todos Buttons, solo nececito que sean 3 caramelos del mismo color
		if(contador >= 3){ // Si hay 3 caramelos, tengo combinacion =)
			std::cout << "\t -- activarCombinacionFila(x=" << x << ", y=" << y << ", todosButtons=" << todosButtons << ", contador=" << contador << ", x_mov="<< x_mov << ", y_mov=" << y_mov << ")" << std::endl;
			std::cout << "\t\t >= 3 " << std::endl;
			huboMovimiento = true;
			if(contador >= 5){
				carameloEspecial = STAR;
				puntos += PUNTOS_BUTTON_CINCO * contador;
			}else if(contador == 4){
				puntos += PUNTOS_BUTTON_CUATRO * 4;
				carameloEspecial = this->horBarColor((Caramelos) this->tablero[this->num2str(x-contador)][this->num2str(y)].asInt());
			}else if(contador == 3){
				puntos += PUNTOS_BUTTON_TRES * 3;
			}

			for(; contador > 0; contador--){
				std::cout << "\t\tapendeo contador:" << contador << std::endl;
				this->tablero[this->num2str(x-contador)][this->num2str(y)] = RELLENAR;
				movimientos.append(this->newMov(x-contador, y, CARAMELO_MOV_LIMBO));
			}
		}
	}else{ // Sino son todos buttons, minimo son 4 caramelos
		if(contador >= 4){
			std::cout << "\t -- activarCombinacionFila(x=" << x << ", y=" << y << ", todosButtons=" << todosButtons << ", contador=" << contador << ", x_mov="<< x_mov << ", y_mov=" << y_mov << ")" << std::endl;
			if(contador >= 5)
				carameloEspecial = STAR;
			else
				carameloEspecial = this->horBarColor((Caramelos) this->tablero[this->num2str(x-contador)][this->num2str(y)].asInt());

			huboMovimiento = true;
			int puntos_por_mov = (contador == 4) ? PUNTOS_BUTTON_CUATRO : PUNTOS_BUTTON_CINCO;
			for(; contador > 0; contador--){ // Tengo que moverme por toda la combinacion activando las bars si las hay
				Caramelos tCar = (Caramelos) this->tablero[this->num2str(x-contador)][this->num2str(y)].asInt();
				puntos += puntos_por_mov;
				this->tablero[this->num2str(x-contador)][this->num2str(y)] = RELLENAR;
				movimientos.append(this->newMov(x-contador, y, CARAMELO_MOV_LIMBO));

				switch(tCar){
					case RED_HORBAR:
					case BLUE_HORBAR:
					case GREEN_HORBAR:
					case PURPLE_HORBAR:
					case YELLOW_HORBAR:
						this->dispararFila(y, movimientos, puntos_por_mov, puntos);
						break;

					case RED_VERBAR:
					case BLUE_VERBAR:
					case GREEN_VERBAR:
					case PURPLE_VERBAR:
					case YELLOW_VERBAR:
						this->dispararColumna(x-contador, movimientos, puntos_por_mov, puntos);
						break;

					default:
						break;
				}
			}
		}
	}

	if(carameloEspecial != RELLENAR){ // Si hubo combinacion especial tengo qe ponerlo en el tablero
		this->tablero[this->num2str(x_mov)][this->num2str(y_mov)] = carameloEspecial;
		movimientos.append(this->newMov(x_mov, y_mov, CARAMELO_MOV_NEW, carameloEspecial));
	}

	return huboMovimiento;
}

bool Tablero::activarCombinacionColumna(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos){
	return this->activarCombinacionColumna(x, y, todosButtons, contador, movimientos, puntos, x, y-contador/2);
};

bool Tablero::activarCombinacionColumna(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos, int x_mov, int y_mov){
	bool huboMovimiento = false;
	Caramelos carameloEspecial = RELLENAR;
	if(todosButtons){
		if(contador >= 3){
			std::cout << "\t -- activarCombinacionColumna(x=" << x << ", y=" << y << ", todosButtons=" << todosButtons << ", contador=" << contador << ", x_mov="<< x_mov << ", y_mov=" << y_mov << ")" << std::endl;
			huboMovimiento = true;
			if(contador >= 5){
				carameloEspecial = STAR;
				puntos += PUNTOS_BUTTON_CINCO * contador;
			}else if(contador == 4){
				puntos += PUNTOS_BUTTON_CUATRO * 4;
				carameloEspecial = this->verBarColor((Caramelos) this->tablero[this->num2str(x)][this->num2str(y-contador)].asInt());
			}else if(contador == 3){
				puntos += PUNTOS_BUTTON_TRES * 3;
			}

			for(; contador > 0; contador--){
				this->tablero[this->num2str(x)][this->num2str(y-contador)] = RELLENAR;
				movimientos.append(this->newMov(x, y-contador, CARAMELO_MOV_LIMBO));
			}
		}
	}else{
		if(contador >= 4){
			std::cout << "\t -- activarCombinacionColumna(x=" << x << ", y=" << y << ", todosButtons=" << todosButtons << ", contador=" << contador << ", x_mov="<< x_mov << ", y_mov=" << y_mov << ")" << std::endl;
			if(contador == 5)
				carameloEspecial = STAR;
			else
				carameloEspecial = this->verBarColor((Caramelos) this->tablero[this->num2str(x)][this->num2str(y-contador)].asInt());

			huboMovimiento = true;
			int puntos_por_mov = (contador == 4) ? PUNTOS_BUTTON_CUATRO : PUNTOS_BUTTON_CINCO;
			for(; contador > 0; contador--){
				Caramelos tCar = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y-contador)].asInt();
				puntos += puntos_por_mov;
				this->tablero[this->num2str(x)][this->num2str(y-contador)] = RELLENAR;
				movimientos.append(this->newMov(x, y-contador, CARAMELO_MOV_LIMBO));

				switch(tCar){
					case RED_HORBAR:
					case BLUE_HORBAR:
					case GREEN_HORBAR:
					case PURPLE_HORBAR:
					case YELLOW_HORBAR:
						this->dispararFila(y-contador, movimientos, puntos_por_mov, puntos);
						break;

					case RED_VERBAR:
					case BLUE_VERBAR:
					case GREEN_VERBAR:
					case PURPLE_VERBAR:
					case YELLOW_VERBAR:
						this->dispararColumna(x, movimientos, puntos_por_mov, puntos);
						break;

					default:
						break;
				}
			}
		}
	}

	if(carameloEspecial != RELLENAR){
		this->tablero[this->num2str(x_mov)][this->num2str(y_mov)] = carameloEspecial;
		movimientos.append(this->newMov(x_mov, y_mov, CARAMELO_MOV_NEW, carameloEspecial));
	}

	return huboMovimiento;
}

Json::Value Tablero::getTablero(){
	Json::Value ret;
	ret["DIM"]["X"] = this->dim_x;
	ret["DIM"]["Y"] = this->dim_y;
	ret["mapa"] = this->nMapa;
	ret["fondo"] = this->pathFondo;
	Json::Value::Members keys = this->mapa["celdas"].getMemberNames();
	for(int i=0; i < keys.size(); i++){
		Json::Value::Members innerkeys = this->mapa["celdas"][keys[i]].getMemberNames();
		for(int j=0; j < innerkeys.size(); j++){
			ret["celdas"][keys[i]][innerkeys[j]]["fondo"] = this->mapa["celdas"][keys[i]][innerkeys[j]]["fondo"];
			ret["celdas"][keys[i]][innerkeys[j]]["pieza"] = this->tablero[keys[i]][innerkeys[j]];
		}
	}

	std::cout << "Tablero que se manda: " << ret << std::endl;
	return ret;
}

int Tablero::doCombinacion(Caramelos car, int x, int y, Json::Value& movimientos){
	// Razonamiento parecido a hayPatrones, solo que en vez de recorrer todo el tablero, solo recorre las fichas cercanas al caramelo
	std::cout << " == doCombinacion( car=" << car << ", x=" << x << ", y=" << y << " )" << std::endl;
	int puntos = 0;
	int contador = 0;
	bool todosButtons = this->esButton(car);
	int x_m;
	for(x_m=x-4 ; x_m < this->dim_x && x_m < x + 4; x_m++){
		if(x_m < 0)
			continue;

		Caramelos esteCar = (Caramelos) this->tablero[this->num2str(x_m)][this->num2str(y)].asInt();
		std::cout << "\t---  x_m" << (x_m) << " y " << y << " car " << car << " y esteCar " << esteCar << std::endl;
		if(this->esMismoColor(car, esteCar)){
			std::cout << "\tson del mismo color" << std::endl;
			contador++;
			if(! this->esButton(esteCar) ){
				std::cout << "\tNo es button" << std::endl;
				todosButtons = false;
			}
		}else{
			std::cout << "\tdistinto" << std::endl;
			if(x_m >= x){
				std::cout << "\t activo combinacion" << std::endl;
				this->activarCombinacionFila(x_m, y, todosButtons, contador, movimientos, puntos, x, y);
			}

			contador = 0;
			todosButtons = this->esButton(car);
			if(x_m > x){
				std::cout << "\t break" << std::endl;
				break;
			}

		}
	}

	this->activarCombinacionFila(x_m, y, todosButtons, contador, movimientos, puntos, x, y);

	contador = 0;
	todosButtons = this->esButton(car);

	int y_m;
	for(y_m=y-4 ; y_m < this->dim_y && y_m < y + 4; y_m++){
		if(y_m < 0)
			continue;
		Caramelos esteCar = (Caramelos) this->tablero[this->num2str(x)][this->num2str(y_m)].asInt();
		if(this->esMismoColor(car, esteCar)){
			contador++;
			if(! this->esButton(esteCar) )
				todosButtons = false;
		}else{
			if(y_m >= y)
				this->activarCombinacionColumna(x, y_m, todosButtons, contador, movimientos, puntos, x, y);

			contador = 0;
			todosButtons = this->esButton(car);
			if(y_m > y)
				break;
		}
	}

	this->activarCombinacionColumna(x, y_m, todosButtons, contador, movimientos, puntos, x, y);

	return puntos;

}

Caramelos Tablero::horBarColor(Caramelos car){
	if(this->esMismoColor(car, RED_HORBAR))
		return RED_HORBAR;
	else if(this->esMismoColor(car, BLUE_HORBAR))
		return BLUE_HORBAR;
	else if(this->esMismoColor(car, GREEN_HORBAR))
		return GREEN_HORBAR;
	else if(this->esMismoColor(car, PURPLE_HORBAR))
		return PURPLE_HORBAR;
	else if(this->esMismoColor(car, YELLOW_HORBAR))
		return YELLOW_HORBAR;

	return RELLENAR;
}

Caramelos Tablero::verBarColor(Caramelos car){
	if(this->esMismoColor(car, RED_VERBAR))
		return RED_VERBAR;
	else if(this->esMismoColor(car, BLUE_VERBAR))
		return BLUE_VERBAR;
	else if(this->esMismoColor(car, GREEN_VERBAR))
		return GREEN_VERBAR;
	else if(this->esMismoColor(car, PURPLE_VERBAR))
		return PURPLE_VERBAR;
	else if(this->esMismoColor(car, YELLOW_VERBAR))
		return YELLOW_VERBAR;

	return RELLENAR;
}

bool Tablero::hayMovimientos(){
	// Vamos a iterar por todas las posiciones, probando los 4 movimientos, si algo sale valido, es qe hay moviento
	for(int y=0; y < this->dim_y; y++){
		for(int x=0; x < this->dim_x; x++){
			if(this->movimientoValido(x, y, CARAMELO_MOV_ARRIBA))
				return true;
			if(this->movimientoValido(x, y, CARAMELO_MOV_DERECHA))
				return true;
			if(this->movimientoValido(x, y, CARAMELO_MOV_ABAJO))
				return true;
			if(this->movimientoValido(x, y, CARAMELO_MOV_IZQ))
				return true;
		}
	}
	return false;
}

#include "cliente.tablerojuego.h"
#include <algorithm>
#include "common.events.h"
#include "common.caramelos_movimientos.h"

TableroJuego::TableroJuego(Json::Value mapa)
	:	tablero(),
		frameTablero(
			mapa["mapa"].asString(),
			Gtk::ALIGN_CENTER,
			Gtk::ALIGN_END, //Eje y
			1.0, //Aspecto
			false //ignorar el aspect del hijo
		)
	{
	//set_size_request(800,600);
	set_title("CandyCrush");
	this->mapa = mapa;
	this->nMapa = mapa["mapa"].asString();
	dimX = getX();
	alto = getX();
	dimY = getY();
	ancho = getY();
	set_size_request(dimY*SIZE,dimX*SIZE);
	this->tablero.set_size_request(dimY*SIZE,dimX*SIZE);
	frameTablero.add(this->tablero);
	//this->dibujarLineas();
	this->crearMatrices();
	this->llenar();
	this->add(frameTablero);
	show_all();
	this->conectarCaramelos();
	clicks=0;
	originX=-1;
	originY=-1;
}

int TableroJuego::getX(){
	int x;
	std::stringstream ss;
	ss << mapa["DIM"]["X"];
	ss >> x;
	return x;
}

int TableroJuego::getY(){
	int y;
	std::stringstream ss;
	ss << mapa["DIM"]["Y"];
	ss >> y;
	return y;
}

void TableroJuego::dibujarLineas(){
	for ( int i= 0 ; i < dimX ; i++ ){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*dimY,SIZE);
		this->tablero.put(*sep_horizontal,0,i*SIZE);
	} 
	for (int j = 0 ; j < dimY ; j++ ) {
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*dimX);
		this->tablero.put(*sep_vertical,j*SIZE,0);
	}
}

void TableroJuego::llenar(){
	std::stringstream sx,sy;
	int idPieza;
	for ( int i = 0 ; i < alto ; i++ ) { // Itera en y
		sy<<i;
		for ( int j = 0 ; j < ancho ; j++ ) { // Itera en x
			sx<<j;
			// this->mapa["celdas"][POS Y][POS X]
			Json::Value celda = this->mapa["celdas"][sx.str()][sy.str()]["pieza"];
			Json::Value celdaFondo = this->mapa["celdas"][sx.str()][sy.str()]["fondo"];
			std::stringstream auxStream ;
			auxStream << celda ;
			auxStream>>idPieza;
			auxStream.str("");
			if (idPieza !=-1){
				if (celdaFondo!=""){
					Gtk::Image* imgFondo = new Gtk::Image(celdaFondo.asString());
					imgFondo->set_size_request(SIZE,SIZE);
					this->tablero.put(*imgFondo,j*SIZE+20,i*SIZE+20);
					imgFondo->show();
				}
				Caramelo* caramelo = CandyFactory::crearCaramelo(idPieza,j,i);
				caramelo->show_all();
				this->tablero.put(*(dynamic_cast<Gtk::Button*>(caramelo)),j*SIZE+20,i*SIZE+20);
				caramelo->setXPos(j*SIZE+20);
				caramelo->setYPos(i*SIZE+20);
				matrizCaramelos[i][j] = caramelo;
				matrizCaramelosAux[i][j] = NULL;
			}else if (idPieza == -1){
				Gtk::Image* hueco = new Gtk::Image("../imagenes/hueco.png");
				hueco->show();
				this->tablero.put(*(hueco),j*SIZE+25,i*SIZE+25);
			}
			sx.str("");
		}
		sy.str("");
	}
}

void TableroJuego::crearMatrices(){
	this->matrizFondos.resize(dimX);
	this->matrizCaramelos.resize(dimX);
	this->matrizCaramelosAux.resize(dimX);
	for ( int i = 0 ; i < dimX ; i++ ) {
		matrizFondos[i].resize(dimY);
		matrizCaramelos[i].resize(dimY);
		matrizCaramelosAux[i].resize(dimY);
	}
}

void TableroJuego::conectarCaramelos(){
	for ( int i = 0 ; i < dimX ; i++ ) {
		for ( int j = 0 ; j < dimY ; j++ ) {
			if (matrizCaramelos[i][j]){
				matrizCaramelos[i][j]->signal_clicked().connect(sigc::bind(sigc::mem_fun(this,&TableroJuego::click),matrizCaramelos[i][j]));
			}
		}
	}
}


// XXX: Los mensajes a emitir tienen que ser de esta forma
// {
//     "event": EVENT_GAME_MISC,
//     "ev-game": EVENT_GAME_MOV,
//     "x": posicion x del caramelo que se mueve,
//     "y": posicion y del caramelo que se mueve,
//     "mov": (CaramelosMovimientos) para donde se mueve
// }
void TableroJuego::click(Caramelo* caramelo){
	bool movimientoInvalido = true;
	clicks++;
	if (!(clicks % 2 )){
		int finalX = caramelo->getX();
		int finalY = caramelo->getY();
		matrizCaramelos[originY][originX]->set_relief(Gtk::RELIEF_NONE);
		if ( (finalX == originX) && (finalY == originY) )
			return;

		//if( (finalX == originX ) || ( finalY == originY) ){
		//	std::cout << "finalX " << finalX << " originX " << originX << " finalY " << finalY << " originY " << originY << std::endl;
		//	this->moverPieza(carameloOrigen, finalX, finalY);
		//	this->moverPieza(caramelo, originX, originY);
		//}

		Json::Value data;
		data["event"] = EVENT_GAME_MISC;
		data["ev-game"] = EVENT_GAME_MOV;
		data["x"] = originX;
		data["y"] = originY;
		if ( finalX == originX ){
			if ( (finalY - originY) == 1){ 
				data["mov"] = CARAMELO_MOV_ABAJO;
				this->m_signal_mensaje.emit(data);
			}else if (finalY - originY == -1){
				data["mov"] = CARAMELO_MOV_ARRIBA;
				this->m_signal_mensaje.emit(data);
			}
		}else if ( finalY == originY){
			if ( (finalX-originX)  == -1 ){
				data["mov"] = CARAMELO_MOV_IZQ;
				this->m_signal_mensaje.emit(data);
			}else if ( (finalX-originX) == 1){
				data["mov"] = CARAMELO_MOV_DERECHA;
				this->m_signal_mensaje.emit(data);
			}
		}

		//if ( finalX == originX ){
		//	step1 = finalY * SIZE +20;
		//	step2 = originY * SIZE +20;
		//	if ( (finalY - originY) == 1){ 
		//		mover2Piezas(originY,finalY,DERECHA,movimientoInvalido);
		//	}else if (finalY - originY == -1){
		//		mover2Piezas(originY,finalY,IZQUIERDA,movimientoInvalido);
		//	}
		//}else if ( finalY == originY){
		//	step1 = (finalX * SIZE) +20;
		//	step2 = (originX * SIZE) +20;
		//	if ( (finalX-originX)  == -1 ){
		//		mover2Piezas(originX , finalX,ARRIBA,movimientoInvalido); // movimietno vertical
		//	}else if ( (finalX-originX) == 1){
		//		mover2Piezas(originX, finalX , ABAJO,movimientoInvalido);
		//	}
		//}
	}else{
		carameloOrigen = caramelo;	
		originX = carameloOrigen->getX();
		originY = carameloOrigen->getY();
		//matrizCaramelos[originX][originY]->set_relief(Gtk::RELIEF_NONE);
		matrizCaramelos[caramelo->getY()][caramelo->getX()]->set_relief(Gtk::RELIEF_NORMAL);
		std::cout << "aprete: " ; matrizCaramelos[caramelo->getX()][caramelo->getY()]->hablar();
	}
}

void TableroJuego::mover2Piezas(int posI,int posF,int DIRECCION,bool volver){
		sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::onTimeout),posI,posF,DIRECCION,volver);
		this->conTimeout = Glib::signal_timeout().connect(my_slot,7);
}

bool TableroJuego::swapBoton(Caramelo* Origen, Caramelo* Final,int DIRECCION){
	switch (DIRECCION){
		case ARRIBA:
			if ( Origen->getX()*SIZE+20 != step2){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),originY*SIZE+20,step1++);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),originY*SIZE+20,step2--);
				return true;
			}else{
				return false;
			}
			break;
		case ABAJO:
			if ( Origen->getX()*SIZE+20 != step2){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),originY*SIZE+20,step1--);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),originY*SIZE+20,step2++);
				return true;
			}else{
				return false;
			}
			break;
		case DERECHA:
			if ( Origen->getY()*SIZE+20 != step2){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),step1--,originX*SIZE+20);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),step2++,originX*SIZE+20);
				return true;
			}else{
				return false;
			}
			break;
		case IZQUIERDA:
			if ( Origen->getY()*SIZE+20 != step2){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),step1++,originX*SIZE+20);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),step2--,originX*SIZE+20);
				return true;
			}else{
				return false;
			}
			break;
	}
}


bool TableroJuego::onTimeout(int posI , int posF , int DIRECCION,bool volver){
	switch ( DIRECCION ){
		case ARRIBA:
			if (swapBoton(matrizCaramelos[posF][originY],matrizCaramelos[originX][originY],DIRECCION))
				return true;
			if (volver){
				swapBoton(matrizCaramelos[originX][originY],matrizCaramelos[posF][originY],ABAJO);
				return false;
			}
			std::swap(matrizCaramelos[posF][originY], matrizCaramelos[posI][originY]);
			matrizCaramelos[posI][originY]->setX(posI);
			matrizCaramelos[posF][originY]->setX(posF);
			return false;
			break;
		case ABAJO:
			if (swapBoton(matrizCaramelos[posF][originY],matrizCaramelos[originX][originY],DIRECCION))
				return true;
			if (volver){
				swapBoton(matrizCaramelos[originX][originY],matrizCaramelos[posF][originY],ARRIBA);
				return false;
			}
			std::swap(matrizCaramelos[posF][originY], matrizCaramelos[posI][originY]);
			matrizCaramelos[posI][originY]->setX(posI);
			matrizCaramelos[posF][originY]->setX(posF);
			return false;
			break;
		case DERECHA:
			if (swapBoton(matrizCaramelos[originX][posF],matrizCaramelos[originX][originY],DIRECCION))
				return true;
			if (volver){
				swapBoton(matrizCaramelos[originX][originY],matrizCaramelos[originX][posF],IZQUIERDA);
				return false;
			}
			std::swap(matrizCaramelos[originX][posF], matrizCaramelos[originX][posI]);
			matrizCaramelos[originX][posI]->setY(posI);
			matrizCaramelos[originX][posF]->setY(posF);
			return false;
			break;
		case IZQUIERDA:
			if (swapBoton(matrizCaramelos[originX][posF],matrizCaramelos[originX][originY],DIRECCION))
				return true;
			if (volver){
				swapBoton(matrizCaramelos[originX][originY],matrizCaramelos[originX][posF],DERECHA);
				return false;
			}
			std::swap(matrizCaramelos[originX][posF], matrizCaramelos[originX][posI]);
			matrizCaramelos[originX][posI]->setY(posI);
			matrizCaramelos[originX][posF]->setY(posF);
			return false;
			break;
		default:
			return false;
	}
}

void TableroJuego::mensaje(Json::Value& data){
	// Aca van a llegar todos los mensajes, en un ppcio solo nos importan los de EVENT_GAME_MOV.
	Json::StaticString def("");
	int code;
	CommonEvents event = EVENT_NONE;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	if(data.get("code", def).isNumeric())
		code = (CommonEvents) data.get("code", def).asInt();

	switch(event){
		case EVENT_GAME_MOV:{
			// EVENT_GAME_MOV, viene con un campo mas "movs" que es un array de objetos, cada uno de estos objetos es el que nececita onMovimiento.
			for(int i=0; i < data["movs"].size() ; i++){
				this->onMovimiento(data["movs"][i]);
			}
			break;
		}
		
		default:
			break;
	}
}

void TableroJuego::onMovimiento(Json::Value& data){
	// Aca voy a tener que hacer cada movimiento.
	int x = data["x"].asInt();
	int y = data["y"].asInt();
	CaramelosMovimientos mov = (CaramelosMovimientos) data["mov"].asInt();
	// Para el tablero tenemos dos matrices, matrizCaramelos y matrizCaramelosAux.
	// La auxiliar inicialmente tiene todos NULL, la idea es primero fijarse si en la auxiliar hay algo en la coordenada, y sino, usar el de matrizCaramelos.
	// Se usa la auxiliar, debido que cuando swapeen dos caramelos, no va a llegar un evento de swap, sino, qe va a decir qe un caramelo se mueve para arriba y dsp otro se mueve para abajo, provocando que se superpongan en la matriz.
	// Pasos para cada movimiento:
	// 1) Busco matrizCaramelosAux[y][x]
	// 2) Si esta en NULL, busco matrizCaramelos[y][x],
	// 3) Calculo x_nueva, y_nueva
	// 4) Miro matrizCaramelos[y_nueva][x_nueva] si hay elemento, lo pongo en matrizCaramelosAux[y_nueva][x_nueva] (Si hay tmb en matrizCaramelosAux, hay algo que no estoy contemplando y mucho meow meow (?) )
	// 4) Pongo caramelo en su nueva posicion en matrizCaramelos[x_nueva][y_nueva]
	// 5) Mando la animacion
	// NOTA: Cuando se hace el NEW, se deberia setear la opacidad del caramelo inicialmente a 0, y mandar una animacion para qe llegue a 1.
	switch(mov){
		case CARAMELO_MOV_ARRIBA:{
			int x2 = x;
			int y2 = y-1;
			this->moveCaramelo(x, y, x2, y2);
			break;
		}
		case CARAMELO_MOV_DERECHA:{
			int x2 = x+1;
			int y2 = y;
			this->moveCaramelo(x, y, x2, y2);
			break;
		}
		case CARAMELO_MOV_ABAJO:{
			int x2 = x;
			int y2 = y+1;
			this->moveCaramelo(x, y, x2, y2);
			break;
		}
		case CARAMELO_MOV_IZQ:{
			int x2 = x-1;
			int y2 = y;
			this->moveCaramelo(x, y, x2, y2);
			break;
		}
		case CARAMELO_MOV_LIMBO:
			this->moveCaramelo(x, y, -1 , -1);
			break;

		case CARAMELO_MOV_NEW:{
			std::cout << "TODO: NEW" << std::endl;
			Caramelos car = (Caramelos) data["car"].asInt();
			Caramelo* caramelo = CandyFactory::crearCaramelo(car,x,y);
			caramelo->setXPos(x*SIZE+20);
			caramelo->setYPos(y*SIZE+20);
			this->tablero.put(*(dynamic_cast<Gtk::Button*>(caramelo)),x*SIZE+20,y*SIZE+20);
			if(this->matrizCaramelos[y][x] != NULL)
				this->matrizCaramelosAux[y][x] = this->matrizCaramelos[y][x];

			this->matrizCaramelos[y][x] = caramelo;

			break;
		 }
	}
}

void TableroJuego::moveCaramelo(int x, int y, int xf, int yf){
	Caramelo* m = this->matrizCaramelosAux[y][x];

	this->matrizCaramelosAux[y][x] = NULL;

	std::cout << "moveCaramelo x: " << x << " y: " << y << " xf: " << xf << " yf: " << yf << std::endl;

	if(m == NULL){
		m = this->matrizCaramelos[y][x];
		this->matrizCaramelos[y][x] = NULL;
	}

	if( xf < 0 || yf < 0){
		// TODO:
		//m->hide();
		//m->set_opacity(0);
		this->moverPieza(m, -1, -1);
	}else{
		if(this->matrizCaramelos[yf][xf] != NULL)
			this->matrizCaramelosAux[yf][xf] = this->matrizCaramelos[yf][xf];

		this->matrizCaramelos[yf][xf] = m;
		this->moverPieza(m, xf, yf);
	}
}

// TODO: faltaria hacer metodo para animar la opacidad (para hacer aparecer las fichas, hacer algo analogo a los metodos de abajo)

void TableroJuego::moverPieza(Caramelo* car, int xF, int yF){
	// TODO: hacer la transformacion de coordenada a posicion de una manera mas limpia
	int posx_final = xF * SIZE + 20;
	int posy_final = yF * SIZE + 20;
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::animationMove), car, posx_final, posy_final ,3, 3);
	Glib::signal_timeout().connect(my_slot,7);
}

bool TableroJuego::animationMove(Caramelo* car, int x_final, int y_final, int step_x, int step_y){
	bool ret = false;

	if(car->mover(x_final, y_final))
		return true;

	if(x_final < 0 || y_final < 0){
		car->hide();
		return false;
	}

	int x = car->getXPos();
	if(x > x_final){
		x -= step_x;
		if(x <= x_final)
			x = x_final;
		else
			ret = true;
	}else if(x < x_final){
		x += step_x;
		if(x >= x_final)
			x = x_final;
		else
			ret = true;
	}

	int y = car->getYPos();
	if(y > y_final){
		y -= step_y;
		if(y <= y_final)
			y = y_final;
		else
			ret = true;
	}else if(y < y_final){
		y += step_y;
		if(y >= y_final)
			y = y_final;
		else
			ret = true;
	}

	//std::cout << "muevo -> x_final " << x_final << " y_final " << y_final << std::endl;
	//std::cout << "muevo -> x " << x << " y " << y << std::endl;
	//std::cout << "termino " << ret << std::endl;
	car->setXPos(x);
	car->setYPos(y);
	this->tablero.move(*(dynamic_cast<Gtk::Button*>(car)), x, y);

	car->setMoviendo(ret, x_final, y_final);
	return ret;
}


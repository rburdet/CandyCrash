#include "cliente.tablerojuego.h"
#include <algorithm>

TableroJuego::TableroJuego(Json::Value mapa)
	:	tablero(),
		frameTablero(getNMapa(mapa),
		Gtk::ALIGN_CENTER,
		Gtk::ALIGN_END, //Eje y
		1.0, //Aspecto
		false //ignorar el aspect del hijo
		)
	{
	//set_size_request(800,600);
	set_title("CandyCrush");
	this->mapa = mapa;
	this->nMapa = getNMapa(mapa);
	dimX = getX();
	dimY = getY();
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

std::string TableroJuego::getNMapa(Json::Value mapa){
	Json::Value::Members keys = mapa.getMemberNames();
	return keys[0];
}

int TableroJuego::getX(){
	int x;
	std::stringstream ss;
	ss << mapa[nMapa]["DIM"]["X"];
	ss >> x;
	return x;
}

int TableroJuego::getY(){
	int y;
	std::stringstream ss;
	ss << mapa[nMapa]["DIM"]["Y"];
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
	std::stringstream sy,sx;
	int idPieza;
	for ( int i = 0 ; i < dimX ; i++ ) {
		sx<<i;
		for ( int j = 0 ; j < dimY ; j++ ) {
			sy<<j;
			Json::Value celda = this->mapa[nMapa]["celdas"][sx.str()][sy.str()]["pieza"];
			std::stringstream auxStream ;
			auxStream << celda ;
			auxStream>>idPieza;
			auxStream.str("");
			if (idPieza !=-1){
				Caramelo* caramelo = CandyFactory::crearCaramelo(idPieza,i,j);
				caramelo->show_all();
				//std::cout << "BORDE PEDORRO" << this->tablero.get_margin_left()<<std::endl;
				this->tablero.put(*(dynamic_cast<Gtk::Button*>(caramelo)),j*SIZE+20,i*SIZE+20);
				matrizCaramelos[i][j] = caramelo;
			}
			sy.str("");
		}
		sx.str("");
	}
}

void TableroJuego::crearMatrices(){
	this->matrizFondos.resize(dimX);
	this->matrizCaramelos.resize(dimX);
	for ( int i = 0 ; i < dimX ; i++ ) {
		matrizFondos[i].resize(dimY);
		matrizCaramelos[i].resize(dimY);
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


void TableroJuego::click(Caramelo* caramelo){
	bool movimientoInvalido = true;
	clicks++;
	if (!(clicks % 2 )){
		int finalX = caramelo->getX();
		int finalY = caramelo->getY();
		matrizCaramelos[originX][originY]->set_relief(Gtk::RELIEF_NONE);
		if ( (finalX == originX) && (finalY == originY) )
			return;
		if ( finalX == originX ){
			step1 = finalY * SIZE +20;
			step2 = originY * SIZE +20;
			if ( (finalY - originY) == 1){ 
				mover2Piezas(originY,finalY,DERECHA,movimientoInvalido);
			}else if (finalY - originY == -1){
				mover2Piezas(originY,finalY,IZQUIERDA,movimientoInvalido);
			}
		}else if ( finalY == originY){
			step1 = (finalX * SIZE) +20;
			step2 = (originX * SIZE) +20;
			if ( (finalX-originX)  == -1 ){
				mover2Piezas(originX , finalX,ARRIBA,movimientoInvalido); // movimietno vertical
			}else if ( (finalX-originX) == 1){
				mover2Piezas(originX, finalX , ABAJO,movimientoInvalido);
			}
		}
	}else{
		carameloOrigen = caramelo;	
		originX = carameloOrigen->getX();
		originY = carameloOrigen->getY();
		//matrizCaramelos[originX][originY]->set_relief(Gtk::RELIEF_NONE);
		matrizCaramelos[caramelo->getX()][caramelo->getY()]->set_relief(Gtk::RELIEF_NORMAL);
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

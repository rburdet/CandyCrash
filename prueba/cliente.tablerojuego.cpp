#include "cliente.tablerojuego.h"

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
				Caramelo* caramelo = CandyFactory::crearCaramelo(idPieza);
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

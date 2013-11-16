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
	this->conectarCaramelos();
	clicks=0;
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
	this->m_senal.resize(dimX);
	for ( int i = 0 ; i < dimX ; i++ ) {
		matrizFondos[i].resize(dimY);
		matrizCaramelos[i].resize(dimY);
		m_senal[i].resize(dimY);
	}
}

void TableroJuego::conectarCaramelos(){
	std::cout << "voy a imprimir la matri " << std::endl;
	for ( int i = 0 ; i < dimX ; i++ ) {
		for ( int j = 0 ; j < dimY ; j++ ) {
			if ( matrizCaramelos[i][j] ){
				std::cout << i << " " << j ;
				m_senal[i][j]=matrizCaramelos[i][j]->signal_clicked().connect(sigc::bind(sigc::mem_fun(this,&TableroJuego::mover),i,j));
			}
		}
		std::cout << std::endl;
	}
}



void TableroJuego::desconectarCaramelos(){
	std::cout << "voy a imprimir la matri " << std::endl;
	for ( int i = 0 ; i < dimX ; i++ ) {
		for ( int j = 0 ; j < dimY ; j++ ) {
			if ( matrizCaramelos[i][j] ){
				std::cout << i << " " << j ;
				m_senal[i][j].disconnect();
			}
		}
		std::cout << std::endl;
	}
}

void TableroJuego::mover(int x , int y){
	clicks++;
	if (!(clicks % 2 )){
	std::cout << x << " " << y << " \t<-\t " << old_x << " " << old_y<< std::endl;
		matrizCaramelos[x][y]->set_relief(Gtk::RELIEF_NONE);
		matrizCaramelos[old_x][old_y]->set_relief(Gtk::RELIEF_NONE);
		if ( (x == old_x) && (y == old_y) )
			return;
		if ( x == old_x ){
			if ( ( (y-old_y)*(y-old_y) ) <= 1 ){
				//mover2Piezas(y ,old_y);
			}
		}else if ( y == old_y){
			if ( ( (x-old_x)*(x-old_x) ) <= 1 ){
				step1 = x * SIZE;
				step2 = old_x * SIZE;
				std::cout << " entrando tengo: old_x : " << old_x <<  "  old_y : " << old_y << std::endl;
				mover2Piezas(old_x , x,VERTICAL); // movimietno vertical
			}
		}
	}else{
		old_x = x ; 
		old_y = y ;
		matrizCaramelos[old_x][old_y]->set_relief(Gtk::RELIEF_NONE);
		matrizCaramelos[x][y]->set_relief(Gtk::RELIEF_NORMAL);
	}
}

void TableroJuego::mover2Piezas(int posI,int posF,int DIRECCION){
	std::cout << "valido" << std::endl;
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::onTimeout),posI,posF,DIRECCION);
	this->conTimeout = Glib::signal_timeout().connect(my_slot,10);
}

bool TableroJuego::onTimeout(int posI , int posF , int DIRECCION){
	std::cout << "pos I : " << posI << " pos F : " << posF << std::endl;
	if ( DIRECCION == VERTICAL){
		if ( (posI - posF ) > 0 ){
			std::cout << "arriba " << std::endl;
			//std::cout << "old y : " << old_y << std::endl;
			//std::cout << "old x : " << old_x << std::endl;
			//std::cout << " posI : " << posI << std::endl;
			//std::cout << " posF : " << posF << std::endl;
			if ( posF*SIZE != step2 ){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(matrizCaramelos[posF][old_y])),old_y*SIZE+20,20+step1++);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(matrizCaramelos[old_x][old_y])),old_y*SIZE+20,20+step2--);
				std::cout << "step1 :" <<step1 << std::endl;
				std::cout << "posF * SIZE:" <<posF*SIZE << std::endl;
				std::cout << "step2 :" <<step2 << std::endl;
				return true;
			}
			std::cout << " saliendo tengo: old_x : " << old_x <<  "  old_y : " << old_y << std::endl;
			this->desconectarCaramelos();
			this->conectarCaramelos();
			old_x = posI;
			//Caramelo* aux = matrizCaramelos[posF][old_y];
			//matrizCaramelos[posF][old_y]->signal_clicked();
			//matrizCaramelos[old_x][old_y]->disconnect();
			std::cout << "salgo " << std::endl;
		}
	}
	if ( (posI - old_x) > 0)
		std::cout << "abajo " << std::endl;
	//if ( (posF - old_y) > 0 )
	//	std::cout << "derecha" << std::endl;
	if ( (posF - old_y) < 0 ) 
		std::cout << " izquierda " <<std::endl;
	//if ((x!=old_x) && (y==old_y)){
	//if ( x==old_x ){
	//	this->tablero.move(*(dynamic_cast<Gtk::Button*>(matrizCaramelos[x][y])),old_y++,x);
	//	this->tablero.move(*(dynamic_cast<Gtk::Button*>(matrizCaramelos[old_x][old_y])),old_y--,x);
	//	return true;
	//}
	return false;
}

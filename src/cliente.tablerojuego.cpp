#include "cliente.tablerojuego.h"
#include <algorithm>
#include "common.events.h"
#include "common.caramelos_movimientos.h"

#define TIMEOUT_TIME (20)

TableroJuego::TableroJuego(Json::Value mapa)
	:	tablero(),
		imagenFondo(),
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
	if (mapa["fondo"].asString() != ""){
		Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file
			(mapa["fondo"].asString(),(dimX+1)*SIZE,(dimX+5)*SIZE); 
		imagenFondo.set(pixbuf);
		imagenFondo.show();
		this->tablero.put(imagenFondo,0,0);
	}
	//imagenFondo.set_size_request(dimY*SIZE,dimX*SIZE);
	set_size_request(dimY*SIZE,dimX*SIZE);
	this->tablero.set_size_request(dimY*SIZE,dimX*SIZE);
	frameTablero.add(this->tablero);
	//this->dibujarLineas();
	this->crearMatrices();
	this->llenar();
	this->add(frameTablero);
	this->show_all();
	this->conectarCaramelos();
	clicks=0;
	colOrigen=-1;
	filaOrigen=-1;
	this->movimientosCount = 0;
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
	for ( int j = 0 ; j < dimY ; j++ ) {
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*dimX);
		this->tablero.put(*sep_vertical,j*SIZE,0);
	}
}

void TableroJuego::llenar(){
	std::stringstream sx,sy;
	int idPieza;
	for ( int j = 0 ; j < ancho ; j++ ) { // Itera en y
		sy<<j;
		for ( int i = 0 ; i < alto ; i++ ) { // Itera en x
			sx<<i;
			// this->mapa["celdas"][POS Y][POS X]
			Json::Value celda = this->mapa["celdas"][sx.str()][sy.str()]["pieza"];
			Json::Value celdaFondo = this->mapa["celdas"][sx.str()][sy.str()]["fondo"];
			std::stringstream auxStream;
			auxStream << celda;
			auxStream>>idPieza;
			auxStream.str("");
			if (idPieza !=-1){
				if (celdaFondo!=""){
					Glib::RefPtr<Gdk::Pixbuf> pixbuf = 
						Gdk::Pixbuf::create_from_file(celdaFondo.asString(),SIZE,SIZE); 
					Gtk::Image* imgFondo = new Gtk::Image(pixbuf);
					imgFondo->show();
					this->tablero.put(*imgFondo,i*SIZE+20,j*SIZE+20);
				}
				Caramelo* caramelo = CandyFactory::crearCaramelo(idPieza,i,j);
				caramelo->show_all();
				this->tablero.put(*(static_cast
							<Gtk::Button*>(caramelo)),i*SIZE+20,j*SIZE+20);
				caramelo->setXPos(i*SIZE+20);
				caramelo->setYPos(j*SIZE+20);
				matrizCaramelos[i][j] = caramelo;
				matrizCaramelosAux[i][j] = NULL;
			}else if (idPieza == -1){
				Gtk::Image* hueco = new Gtk::Image("../share/candycrash/imagenes/hueco.png");
				hueco->show();
				this->tablero.put(*(hueco),i*SIZE+25,j*SIZE+25);
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
				matrizCaramelos[i][j]->signal_clicked().connect(sigc::bind(
							sigc::mem_fun(this,&TableroJuego::click),
							matrizCaramelos[i][j]));
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
	bool movimientoInvalido = false;
	clicks++;
	if (!(clicks % 2 )){
		int colFinal = caramelo->getX();
		int filaFinal = caramelo->getY();
		matrizCaramelos[colOrigen][filaOrigen]->set_relief(Gtk::RELIEF_NONE);
		if ( (colFinal == colOrigen) && (filaFinal == filaOrigen) )
			return;
		Json::Value data;
		data["event"] = EVENT_GAME_MISC;
		data["ev-game"] = EVENT_GAME_MOV;
		data["x"] = colOrigen;
		data["y"] = filaOrigen;
		if ( colFinal == colOrigen ){
			step1 = filaFinal * SIZE +20;
			step2 = filaOrigen * SIZE +20;
			if ((filaFinal - filaOrigen) == 1){ 
				data["mov"] = CARAMELO_MOV_ABAJO;
				this->m_signal_mensaje.emit(data);
			}else if (filaFinal - filaOrigen == -1){
				data["mov"] = CARAMELO_MOV_ARRIBA;
				this->m_signal_mensaje.emit(data);
			}
		}else if (filaFinal == filaOrigen){
			step1 = (colFinal * SIZE) +20;
			step2 = (colOrigen * SIZE) +20;
			if ( (colFinal-colOrigen)  == -1 ){
				data["mov"] = CARAMELO_MOV_IZQ;
				this->m_signal_mensaje.emit(data);
			}else if ((colFinal-colOrigen) == 1){
				data["mov"] = CARAMELO_MOV_DERECHA;
				this->m_signal_mensaje.emit(data);
			}
		}
	}else{
		carameloOrigen = caramelo;	
		colOrigen = carameloOrigen->getX();
		filaOrigen = carameloOrigen->getY();
		matrizCaramelos[caramelo->getX()][caramelo->getY()]->set_relief
			(Gtk::RELIEF_NORMAL);
	}
}

void TableroJuego::mensaje(Json::Value& data){
	Json::StaticString def("");
	int code;
	CommonEvents event = EVENT_NONE;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	if(data.get("code", def).isNumeric())
		code = (CommonEvents) data.get("code", def).asInt();

	switch(event){
		case EVENT_GAME_MOV:{
			for(int i=0; i < data["movs"].size() ; i++){
				//this->onMovimiento(data["movs"][i]);
				this->movimientos.push_back(data["movs"][i]);
			}
			this->triggerMovimientos();
			break;
		}
		
		default:
			break;
	}
}

void TableroJuego::triggerMovimientos(){
	if(this->movimientosCount != 0 || this->movimientos.size() == 0)
		return;

	CaramelosMovimientos mov = 
		(CaramelosMovimientos) this->movimientos[0]["mov"].asInt();
	this->onMovimiento(this->movimientos[0]);
	this->movimientos.erase(this->movimientos.begin());

	while(this->movimientos.size()){
		bool last = false;
		CaramelosMovimientos tMov = (CaramelosMovimientos) 
			this->movimientos[0]["mov"].asInt();
		switch(mov){
			case CARAMELO_MOV_ARRIBA:
			case CARAMELO_MOV_DERECHA:
			case CARAMELO_MOV_ABAJO:
			case CARAMELO_MOV_IZQ:
			case CARAMELO_MOV_NEW:
				if(tMov == CARAMELO_MOV_NEW || tMov == CARAMELO_MOV_ARRIBA || 
						tMov == CARAMELO_MOV_DERECHA || tMov == CARAMELO_MOV_ABAJO 
						|| tMov == CARAMELO_MOV_IZQ){
					this->onMovimiento(this->movimientos[0]);
					this->movimientos.erase(this->movimientos.begin());
				}else{
					last = true;
				}

				break;
			case CARAMELO_MOV_LIMBO:
				if(mov == tMov){
					this->onMovimiento(this->movimientos[0]);
					this->movimientos.erase(this->movimientos.begin());
				}else{
					last = true;
				}
				break;
		}
		if(last)
			break;
	}
}

void TableroJuego::onMovimiento(Json::Value& data){
	// Aca voy a tener que hacer cada movimiento.
	int x = data["x"].asInt();
	int y = data["y"].asInt();
	CaramelosMovimientos mov = (CaramelosMovimientos) data["mov"].asInt();
	this->movimientosCount++;
	// Para el tablero tenemos dos matrices, matrizCaramelos y matrizCaramelosAux.
	// La auxiliar inicialmente tiene todos NULL, la idea es primero fijarse 
	// si en la auxiliar hay algo en la coordenada, y sino, usar el de 
	// matrizCaramelos.
	// Se usa la auxiliar, debido que cuando swapeen dos caramelos, no va a 
	// llegar un evento de swap, sino, qe va a decir qe un caramelo se mueve 
	// para arriba y dsp otro se mueve para abajo, provocando que se superponga
	// n en la matriz.
	// Pasos para cada movimiento:
	// 1) Busco matrizCaramelosAux[y][x]
	// 2) Si esta en NULL, busco matrizCaramelos[y][x],
	// 3) Calculo x_nueva, y_nueva
	// 4) Miro matrizCaramelos[y_nueva][x_nueva] si hay elemento, lo pongo en
	// matrizCaramelosAux[y_nueva][x_nueva] (Si hay tmb en matrizCaramelosAux,
	// hay algo que no estoy contemplando y mucho meow meow (?) )
	// 4) Pongo caramelo en su nueva posicion en matrizCaramelos[x_nueva][y_nueva]
	// 5) Mando la animacion
	// NOTA: Cuando se hace el NEW, se deberia setear la opacidad del 
	// caramelo inicialmente a 0, y mandar una animacion para qe llegue a 1.
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
			Caramelos car = (Caramelos) data["car"].asInt();
			Caramelo* caramelo = CandyFactory::crearCaramelo(car,x,y);
			caramelo->setXPos(x*SIZE+20);
			caramelo->setYPos(y*SIZE+20);
			caramelo->set_opacity(0);
			this->tablero.put(*(static_cast<Gtk::Button*>(caramelo)),x*SIZE+20
					,y*SIZE+20);
			caramelo->show();
			this->matrizCaramelosAux[x][y] = this->matrizCaramelos[x][y];
			this->matrizCaramelos[x][y] = caramelo;
			caramelo->signal_clicked().connect(sigc::bind(sigc::mem_fun(this,
							&TableroJuego::click),caramelo));
			this->aparecer(caramelo);
			break;
		 }
	}
}

void TableroJuego::moveCaramelo(int x, int y, int xf, int yf){
	Caramelo* m = this->matrizCaramelosAux[x][y];
	this->matrizCaramelosAux[x][y] = NULL;
	if (m == NULL){
		m = this->matrizCaramelos[x][y];
		this->matrizCaramelos[x][y] = NULL;
	}

	if (xf < 0 || yf < 0){
		if(m){
			this->esfumar(m);
		}else{
			std::cout << "ES NULL" << std::endl;
			this->movimientosCount--;
			this->triggerMovimientos();
		}
	}else{
		if (this->matrizCaramelos[xf][yf] != NULL)
			this->matrizCaramelosAux[xf][yf] = this->matrizCaramelos[xf][yf];

		this->matrizCaramelos[xf][yf] = m;
		this->moverPieza(m, xf, yf);
		this->matrizCaramelos[xf][yf]->setX(xf);
		this->matrizCaramelos[xf][yf]->setY(yf);
	}
}


void TableroJuego::moverPieza(Caramelo* car, int xF, int yF){
	int posx_final = (xF * SIZE) + 20;
	int posy_final = (yF * SIZE) + 20;
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,
				&TableroJuego::animationMove), car, posx_final, posy_final ,3, 3);
	Glib::signal_timeout().connect(my_slot, TIMEOUT_TIME);
}

bool TableroJuego::animationMove(Caramelo* car, int x_final, int y_final,
		int step_x, int step_y){
	bool ret = false;

	if(car->mover(x_final, y_final))
		return true;

	if(x_final < 0 || y_final < 0){
		//car->hide();
		//this->esfumar(car);
		this->movimientosCount--;
		this->triggerMovimientos();
		return false;
	}

	int x = car->getXPos();
	if (x > x_final){
		x -= step_x;
		if(x <= x_final)
			x = x_final;
		else
			ret = true;
	}else if (x < x_final){
		x += step_x;
		if(x >= x_final)
			x = x_final;
		else
			ret = true;
	}
	
	int y = car->getYPos();
	if (y > y_final){
		y -= step_y;
		if(y <= y_final)
			y = y_final;
		else
			ret = true;
	}else if (y < y_final){
		y += step_y;
		if(y >= y_final)
			y = y_final;
		else
			ret = true;
	}
	car->setXPos(x);
	car->setYPos(y);
	car->get_window()->raise();
	this->tablero.move(*(static_cast<Gtk::Button*>(car)), x, y);
	car->setMoviendo(ret, x_final, y_final);
	if (ret == false){
		this->movimientosCount--;
		this->triggerMovimientos();
	}
	return ret;
}

void TableroJuego::aparecer(Caramelo* caramelo){
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,
				&TableroJuego::onAclarar),caramelo);
	this->conTimeout = Glib::signal_timeout().connect(my_slot, TIMEOUT_TIME);
}

bool TableroJuego::onAclarar(Caramelo* caramelo){
	if (caramelo->fullyVisible()){
		this->movimientosCount--;
		this->triggerMovimientos();
		return false;
	}

	caramelo->hacerAparecer();
	return true;
}

void TableroJuego::esfumar(Caramelo* caramelo){
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,
				&TableroJuego::onOpacar), caramelo);
	this->conTimeout = Glib::signal_timeout().connect(my_slot, TIMEOUT_TIME);
}

bool TableroJuego::onOpacar(Caramelo* caramelo){
	if (caramelo->visible()){
		caramelo->opacar();
		return true;
	}
	//delete caramelo;
	//bajar();
	this->movimientosCount--;
	this->triggerMovimientos();
	return false;
}

bool TableroJuego::onClose(){
	Gtk::MessageDialog dialog(*this,"ALERTA",false,
			Gtk::MESSAGE_WARNING,Gtk::BUTTONS_OK);
	dialog.set_secondary_text("Para cerrar termine la partida");
	dialog.run();
	return true;
}

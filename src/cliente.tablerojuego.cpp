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
	Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(mapa["fondo"].asString(),(dimX+1)*SIZE,(dimX+5)*SIZE); 
	imagenFondo.set(pixbuf);
	//imagenFondo.set_size_request(dimY*SIZE,dimX*SIZE);
	set_size_request(dimY*SIZE,dimX*SIZE);
	this->tablero.set_size_request(dimY*SIZE,dimX*SIZE);
	imagenFondo.show();
	frameTablero.add(this->tablero);
	this->tablero.put(imagenFondo,0,0);
	//this->dibujarLineas();
	this->crearMatrices();
	this->llenar();
	this->add(frameTablero);
	show_all();
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
	for (int j = 0 ; j < dimY ; j++ ) {
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
			std::stringstream auxStream ;
			auxStream << celda ;
			auxStream>>idPieza;
			auxStream.str("");
			if (idPieza !=-1){
				if (celdaFondo!=""){
					Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file(celdaFondo.asString(),SIZE,SIZE); 
					Gtk::Image* imgFondo = new Gtk::Image(pixbuf);
					imgFondo->set_size_request(SIZE,SIZE);
					this->tablero.put(*imgFondo,i*SIZE+20,j*SIZE+20);
					imgFondo->show();
				}
				Caramelo* caramelo = CandyFactory::crearCaramelo(idPieza,i,j);
				caramelo->show_all();
				this->tablero.put(*(dynamic_cast<Gtk::Button*>(caramelo)),i*SIZE+20,j*SIZE+20);
				caramelo->setXPos(i*SIZE+20);
				caramelo->setYPos(j*SIZE+20);
				matrizCaramelos[i][j] = caramelo;
				matrizCaramelosAux[i][j] = NULL;
			}else if (idPieza == -1){
				Gtk::Image* hueco = new Gtk::Image("../imagenes/hueco.png");
				hueco->show();
				this->tablero.put(*(hueco),i*SIZE+25,j*SIZE+25);
			}
			//std::cout << "en : " << i << " " << j << " : " << idPieza << " " << matrizCaramelos[i][j]->getId() << " \t";
			sx.str("");
		}
		//std::cout<< std::endl;
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
	std::cout << " aprete un caramelo de id ::: " << caramelo->getId() << std::endl;
	std::cout << "Columna origen : " << colOrigen<<"\t";
	std::cout << "Fila Origen : " << filaOrigen<<std::endl;
	bool movimientoInvalido = false;
	clicks++;
	if (!(clicks % 2 )){
		int colFinal = caramelo->getX();
		int filaFinal = caramelo->getY();
		std::cout << "colFinal : " << colFinal<<"\t";
		std::cout << "filaFinal : " << filaFinal<<std::endl;
		matrizCaramelos[colOrigen][filaOrigen]->set_relief(Gtk::RELIEF_NONE);
		if ( (colFinal == colOrigen) && (filaFinal == filaOrigen) )
			return;

		//if( (colFinal == colOrigen ) || ( filaFinal == filaOrigen) ){
		//	std::cout << "colFinal " << colFinal << " colOrigen " << colOrigen << " filaFinal " << filaFinal << " filaOrigen " << filaOrigen << std::endl;
		//	this->moverPieza(carameloOrigen, colFinal, filaFinal);
		//	this->moverPieza(caramelo, colOrigen, filaOrigen);
		//}

		Json::Value data;
		data["event"] = EVENT_GAME_MISC;
		data["ev-game"] = EVENT_GAME_MOV;
		data["x"] = colOrigen;
		data["y"] = filaOrigen;
		if ( colFinal == colOrigen ){
			step1 = filaFinal * SIZE +20;
			step2 = filaOrigen * SIZE +20;
			if ( (filaFinal - filaOrigen) == 1){ 
				//std::cout << "muevo ABAJO " << std::endl;
				data["mov"] = CARAMELO_MOV_ABAJO;
				this->m_signal_mensaje.emit(data);
				//mover2Piezas(filaOrigen, filaFinal , ABAJO,movimientoInvalido);
			}else if (filaFinal - filaOrigen == -1){
				//std::cout << "muevo ARRIBA " << std::endl;
				data["mov"] = CARAMELO_MOV_ARRIBA;
				this->m_signal_mensaje.emit(data);
				//mover2Piezas(filaOrigen, filaFinal , ARRIBA,movimientoInvalido);
			}
		}else if ( filaFinal == filaOrigen){
			step1 = (colFinal * SIZE) +20;
			step2 = (colOrigen * SIZE) +20;
			if ( (colFinal-colOrigen)  == -1 ){
				//std::cout << "muevo IZQUIERDA " << std::endl;
				data["mov"] = CARAMELO_MOV_IZQ;
				this->m_signal_mensaje.emit(data);
				//mover2Piezas(colOrigen,colFinal,IZQUIERDA,movimientoInvalido);
			}else if ( (colFinal-colOrigen) == 1){
					//std::cout << "muevo DERECHA " << std::endl;
				data["mov"] = CARAMELO_MOV_DERECHA;
				this->m_signal_mensaje.emit(data);
				//mover2Piezas(colOrigen,colFinal,DERECHA,movimientoInvalido);
			}
		}

		//if ( colFinal == colOrigen ){
		//	step1 = filaFinal * SIZE +20;
		//	step2 = filaOrigen * SIZE +20;
		//	if ( (filaFinal - filaOrigen) == 1){ 
		//		mover2Piezas(filaOrigen,filaFinal,DERECHA,movimientoInvalido);
		//	}else if (filaFinal - filaOrigen == -1){
		//		mover2Piezas(filaOrigen,filaFinal,IZQUIERDA,movimientoInvalido);
		//	}
		//}else if ( filaFinal == filaOrigen){
		//	step1 = (colFinal * SIZE) +20;
		//	step2 = (colOrigen * SIZE) +20;
		//	if ( (colFinal-colOrigen)  == -1 ){
		//		mover2Piezas(colOrigen , colFinal,ARRIBA,movimientoInvalido); // movimietno vertical
		//	}else if ( (colFinal-colOrigen) == 1){
		//		mover2Piezas(colOrigen, colFinal , ABAJO,movimientoInvalido);
		//	}
		//}
	}else{
		carameloOrigen = caramelo;	
		colOrigen = carameloOrigen->getX();
		filaOrigen = carameloOrigen->getY();
		//matrizCaramelos[colOrigen][filaOrigen]->set_relief(Gtk::RELIEF_NONE);
		matrizCaramelos[caramelo->getX()][caramelo->getY()]->set_relief(Gtk::RELIEF_NORMAL);
		//std::cout << "aprete: " ; matrizCaramelos[caramelo->getX()][caramelo->getY()]->hablar();
	}
}


//TODO: no me estan volviendo bien las fichitas, ver una solucion copada
//metodo ASOMAR a mitad de camino
//void TableroJuego::mover2Piezas(int posI,int posF,int DIRECCION,bool volver){
//		sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::onTimeout),posI,posF,DIRECCION,volver);
//		this->conTimeout = Glib::signal_timeout().connect(my_slot, TIMEOUT_TIME);
//}

/*bool TableroJuego::onTimeout(int posI , int posF , int DIRECCION,bool volver){
	switch ( DIRECCION ){
		case ARRIBA:
			if (swapBoton(matrizCaramelos[colOrigen][posI],matrizCaramelos[colOrigen][posF],DIRECCION))
				return true;
			if (volver){
				swapBoton(matrizCaramelos[colOrigen][posI],matrizCaramelos[colOrigen][posF],DIRECCION);
				return false;
			}
			std::swap(matrizCaramelos[colOrigen][posF], matrizCaramelos[colOrigen][posI]);
			matrizCaramelos[colOrigen][posI]->setX(posI);
			matrizCaramelos[colOrigen][posF]->setX(posF);
			return false;
			break;
		case ABAJO:
			if (swapBoton(matrizCaramelos[colOrigen][posI],matrizCaramelos[colOrigen][posF],DIRECCION))
				return true;
			if (volver){
				std::cout << "tengo que volver " << std::endl;
				swapBoton(matrizCaramelos[colOrigen][filaOrigen],matrizCaramelos[colOrigen][posF],ARRIBA);
				return false;
			}
			std::swap(matrizCaramelos[colOrigen][posF], matrizCaramelos[colOrigen][posI]);
			matrizCaramelos[colOrigen][posI]->setX(posI);
			matrizCaramelos[colOrigen][posF]->setX(posF);
			return false;
			break;
		case DERECHA:
			if (swapBoton(matrizCaramelos[posI][filaOrigen],matrizCaramelos[posF][filaOrigen],DIRECCION))
				return true;
			if (volver){
				std::cout << "tengo que volver " << std::endl;
				swapBoton(matrizCaramelos[colOrigen][filaOrigen],matrizCaramelos[posF][filaOrigen],IZQUIERDA);
				return false;
			}
			std::swap(matrizCaramelos[posF][filaOrigen], matrizCaramelos[posI][filaOrigen]);
			matrizCaramelos[posI][filaOrigen]->setY(posI);
			matrizCaramelos[posF][filaOrigen]->setY(posF);
			return false;
			break;
		case IZQUIERDA:
			if (swapBoton(matrizCaramelos[posI][filaOrigen],matrizCaramelos[posF][filaOrigen],DIRECCION))
				return true;
			if (volver){
				swapBoton(matrizCaramelos[posI][filaOrigen],matrizCaramelos[posF][filaOrigen],DERECHA);
				return false;
			}
			std::swap(matrizCaramelos[posF][filaOrigen], matrizCaramelos[posI][filaOrigen]);
			std::cout << " termine de mover y tengo en : " << posF << " " << filaOrigen << " " << matrizCaramelos[posF][filaOrigen]->getId() << std::endl;
			std::cout << "con : " <<matrizCaramelos[posF][filaOrigen]->getX() << " " << matrizCaramelos[posF][filaOrigen]->getY() << std::endl;
			matrizCaramelos[posI][filaOrigen]->setY(posI);
			matrizCaramelos[posF][filaOrigen]->setY(posF);
			return false;
			break;
		default:
			return false;
	}
}*/

bool TableroJuego::asomar(Caramelo* Origen, Caramelo* Final,int DIRECCION){
	
	//switch(DIRECCION){
	//	case ARRIBA:
	//			std::cout << "me muevo" << std::endl;
	//			if ( Origen->getX()*SIZE+20 != Final->getXPos()){
	//				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),colOrigen*SIZE+20,Final->getXPos());
	//				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),colOrigen*SIZE+20,Origen->getXPos());
	//				auxFin=Final->getXPos()+1;
	//				auxOri=Origen->getXPos()-1;
	//				Final->setXPos(auxFin);
	//				Origen->setXPos(auxOri);
	//		}
	//}
}

/*bool TableroJuego::swapBoton(Caramelo* Origen, Caramelo* Final,int DIRECCION){
	switch (DIRECCION){
		case ARRIBA:
			if ( Origen->getX()*SIZE+20 != Final->getXPos()){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),colOrigen*SIZE+20,Origen->getXPos());
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),colOrigen*SIZE+20,Final->getXPos());
				Final->setXPos(Final->getXPos()+1);
				Origen->setXPos(Origen->getXPos()-1);
				return true;
			}else{
				return false;
			}
			break;
		case ABAJO:
			if ( Origen->getX()*SIZE+20 != Final->getXPos()){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),colOrigen*SIZE+20,Origen->getXPos());
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),colOrigen*SIZE+20,Final->getXPos());
				Final->setXPos(Final->getXPos()-1);
				Origen->setXPos(Origen->getXPos()+1);
				return true;
			}else{
				return false;
			}
			break;
		case DERECHA:
			if ( Origen->getY()*SIZE+20 != Final->getYPos()){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),Origen->getYPos(),filaOrigen*SIZE+20);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),Final->getYPos(),filaOrigen*SIZE+20);
				Final->setYPos(Final->getYPos()-1);
				Origen->setYPos(Origen->getYPos()+1);
				return true;
			}else{
				return false;
			}
			break;
		case IZQUIERDA:
			if ( Origen->getY()*SIZE+20 != Final->getYPos()){
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Origen)),Origen->getYPos(),filaOrigen*SIZE+20);
				this->tablero.move(*(dynamic_cast<Gtk::Button*>(Final)),Origen->getYPos(),filaOrigen*SIZE+20);
				Final->setYPos(Final->getYPos()+1);
				Origen->setYPos(Origen->getYPos()-1);
				return true;
			}else{
				return false;
			}
			break;
	}
}*/


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

	CaramelosMovimientos mov = (CaramelosMovimientos) this->movimientos[0]["mov"].asInt();
	this->onMovimiento(this->movimientos[0]);
	this->movimientos.erase(this->movimientos.begin());

	while(this->movimientos.size()){
		bool last = false;
		CaramelosMovimientos tMov = (CaramelosMovimientos) this->movimientos[0]["mov"].asInt();
		switch(mov){
			case CARAMELO_MOV_ARRIBA:
			case CARAMELO_MOV_DERECHA:
			case CARAMELO_MOV_ABAJO:
			case CARAMELO_MOV_IZQ:
			case CARAMELO_MOV_NEW:
				if(tMov == CARAMELO_MOV_NEW || tMov == CARAMELO_MOV_ARRIBA || tMov == CARAMELO_MOV_DERECHA || tMov == CARAMELO_MOV_ABAJO || tMov == CARAMELO_MOV_IZQ){
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
	//std::cout << " se quiso mover : " << x << "\t" << y << std::endl;
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
			//this->mover2Piezas(y,y2,ARRIBA,false);
			break;
		}
		case CARAMELO_MOV_DERECHA:{
			int x2 = x+1;
			int y2 = y;
			this->moveCaramelo(x, y, x2, y2);
			//this->mover2Piezas(x,x2,DERECHA,false);
			break;
		}
		case CARAMELO_MOV_ABAJO:{
			int x2 = x;
			int y2 = y+1;
			this->moveCaramelo(x, y, x2, y2);
			//this->mover2Piezas(y,y2,ABAJO,false);
			break;
		}
		case CARAMELO_MOV_IZQ:{
			int x2 = x-1;
			int y2 = y;
			this->moveCaramelo(x, y, x2, y2);
			//this->mover2Piezas(x,x2,IZQUIERDA,false);
			break;
		}
		case CARAMELO_MOV_LIMBO:
			this->moveCaramelo(x, y, -1 , -1);
			break;

		case CARAMELO_MOV_NEW:{
			//std::cout << "TODO: NEW" << std::endl;
			Caramelos car = (Caramelos) data["car"].asInt();
			Caramelo* caramelo = CandyFactory::crearCaramelo(car,x,y);
			caramelo->setXPos(x*SIZE+20);
			caramelo->setYPos(y*SIZE+20);
			caramelo->set_opacity(0);
			caramelo->show();
			this->tablero.put(*(dynamic_cast<Gtk::Button*>(caramelo)),x*SIZE+20,y*SIZE+20);
//			if(this->matrizCaramelos[y][x] != NULL)
//				this->matrizCaramelosAux[y][x] = this->matrizCaramelos[y][x];
//
//			this->matrizCaramelos[y][x] = caramelo;

			//CAMBIOS HECHOS POR MI <3 if(this->matrizCaramelos[x][y] != NULL)
				this->matrizCaramelosAux[x][y] = this->matrizCaramelos[x][y];

			this->matrizCaramelos[x][y] = caramelo;

			caramelo->signal_clicked().connect(sigc::bind(sigc::mem_fun(this,&TableroJuego::click),caramelo));

			this->aparecer(caramelo);


			break;
		 }
	}
}
//MODIFICADO POR MI <3
void TableroJuego::moveCaramelo(int x, int y, int xf, int yf){
	Caramelo* m = this->matrizCaramelosAux[x][y];

	this->matrizCaramelosAux[x][y] = NULL;

	//std::cout << "moveCaramelo x: " << x << " y: " << y << " xf: " << xf << " yf: " << yf << std::endl;

	if(m == NULL){
		m = this->matrizCaramelos[x][y];
		this->matrizCaramelos[x][y] = NULL;
	}

	if( xf < 0 || yf < 0){
		if(m){
			this->esfumar(m);
		}else{
			std::cout << "ES NULL" << std::endl;
			this->movimientosCount--;
			this->triggerMovimientos();
		}
	}else{
		if(this->matrizCaramelos[xf][yf] != NULL)
			this->matrizCaramelosAux[xf][yf] = this->matrizCaramelos[xf][yf];

		this->matrizCaramelos[xf][yf] = m;
		this->moverPieza(m, xf, yf);
		// ULTIMO HECHO ... Se esta poniendo mal las coordenadas , FALTA !  
		this->matrizCaramelos[xf][yf]->setX(xf);
		this->matrizCaramelos[xf][yf]->setY(yf);

	}
}


//void TableroJuego::moveCaramelo(int x, int y, int xf, int yf){
//	Caramelo* m = this->matrizCaramelosAux[y][x];
//
//	this->matrizCaramelosAux[y][x] = NULL;
//
//	std::cout << "moveCaramelo x: " << x << " y: " << y << " xf: " << xf << " yf: " << yf << std::endl;
//
//	if(m == NULL){
//		m = this->matrizCaramelos[y][x];
//		this->matrizCaramelos[y][x] = NULL;
//	}
//
//	if( xf < 0 || yf < 0){
//		// TODO:
//		//m->hide();
//		//m->set_opacity(0);
//		this->moverPieza(m, -1, -1);
//	}else{
//		if(this->matrizCaramelos[yf][xf] != NULL)
//			this->matrizCaramelosAux[yf][xf] = this->matrizCaramelos[yf][xf];
//
//		this->matrizCaramelos[yf][xf] = m;
//		this->moverPieza(m, xf, yf);
//	}
//}

// TODO: faltaria hacer metodo para animar la opacidad (para hacer aparecer las fichas, hacer algo analogo a los metodos de abajo)

void TableroJuego::moverPieza(Caramelo* car, int xF, int yF){
	std::cout << "antes de mover xf: " << xF << "  yF: " << yF << std::endl;
	// TODO: hacer la transformacion de coordenada a posicion de una manera mas limpia
	int posx_final = (xF * SIZE) + 20;
	int posy_final = (yF * SIZE) + 20;
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::animationMove), car, posx_final, posy_final ,3, 3);
	Glib::signal_timeout().connect(my_slot, TIMEOUT_TIME);
}

bool TableroJuego::animationMove(Caramelo* car, int x_final, int y_final, int step_x, int step_y){
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
	//std::cout << " tengo que mover x: " << x <<" = "<< car->getX()<< " a : " << x_final<< std::endl;
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
	//std::cout << " tengo que mover y : " << y <<" = "<< car->getY()<< " a : " << y_final << std::endl;
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
	if(ret == false){
		this->movimientosCount--;
		this->triggerMovimientos();
	}
	return ret;
}

void TableroJuego::aparecer(Caramelo* caramelo){
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::onAclarar),caramelo);
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
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this,&TableroJuego::onOpacar), caramelo);
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

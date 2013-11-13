#include "tablero.h"

#include <gtkmm.h>

#define MAX_COLS 14
#define MIN_COLS 8
#define MIN_FILAS 8
#define MAX_FILAS 14
#define SIZE 40
#define OFFSET 20


Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	celdaInteres=NULL;
	builder->get_widget("f_tablero",tablero);
	builder->get_widget("event_tablero",eventos_tablero);
	this->eventos_tablero->set_events(Gdk::BUTTON_PRESS_MASK);
	cantFilas=MIN_FILAS;
	cantColumnas=MIN_COLS;
	agregarFilas(cantFilas);
	agregarColumnas(cantColumnas);
	ultFilClick=MAX_FILAS;
	ultColClick=MAX_COLS;
	celdaInteres=NULL;
	colInteres=NULL;
	actualizarMatriz(cantFilas,cantColumnas);
	this->eventos_tablero->signal_button_press_event().connect(sigc::mem_fun(*this,&Tablero::on_click_tablero));
	
}

Tablero::~Tablero(){
	this->borrarSeps();
	for ( unsigned int i = 0 ; i < matrizCeldas.size() ; i++){
		for ( unsigned int j = 0 ; j < matrizCeldas[0].size() ; j++ ){
			delete matrizCeldas[i][j];
		}
	}
}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	//El +1 aca es xq se dibuja un pedazo afuera
	//el tama;o real es sin ese +1 
	int X = spin_x->get_value_as_int() + 1;
	if 	( X > cantFilas ){
		cantFilas=X;
		agregarFilas(X);
		alargarColumnas(X);
	}else if( X < cantFilas ){
		borrarSeps();
	}
	cantFilas=X;
	actualizarMatriz(cantFilas-1,cantColumnas-1);
}

void Tablero::on_cordy_changed(Gtk::SpinButton* spin_y){
	int Y = spin_y->get_value_as_int() + 1;
	if ( Y > cantColumnas ) {
		cantColumnas=Y;
		agregarColumnas(Y);
		alargarFilas(Y);
	}else if ( Y < cantColumnas ) {
		borrarSeps();
	}
	cantColumnas=Y;
	actualizarMatriz(cantFilas-1,cantColumnas-1);
}

void Tablero::alargarFilas(int Y){
	for (int i = 0 ; i < cantFilas ; i++){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*Y,SIZE);
		this->tablero->put(*sep_horizontal,0,i*SIZE);
		this->tablero->show_all();
	}
}

void Tablero::alargarColumnas(int X){
	for ( int i = 0 ; i < cantColumnas ; i++ ){
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*X);
		this->tablero->put(*sep_vertical,i*SIZE,0);
		this->tablero->show_all();
	}
}

void Tablero::agregarFilas(int X){
	for ( int i= 0 ; i < X ; i++ ){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*cantColumnas,SIZE);
		this->tablero->put(*sep_horizontal,0,i*SIZE);
	} 
}

void Tablero::agregarColumnas(int Y){
	columnas.clear();
	for ( int i = 0 ; i < Y ; i++ ){
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*cantFilas);
		this->tablero->put(*sep_vertical,i*SIZE,0);
		Gtk::Button * button = new Gtk::Button();
		Columna* col = new Columna(Y, button);
		if ( i == Y-1){
			this->tablero->show_all();
			break;
		}
		this->columnas.push_back(col);
		columnas[i]->get_boton()->signal_clicked().connect(bind(sigc::mem_fun(*this,&Tablero::on_click_boton_tablero),i));
		this->tablero->put(*button,(i+1)*SIZE,0);
	} 
}

//TODO: BORRA T0D0, ver si se puede mejorar para que solo borre lo que agrego
void Tablero::borrarSeps(){
	Glib::ListHandle<Gtk::Widget*> separadores = this->tablero->get_children();
	Glib::ListHandle<Gtk::Widget*>::iterator it = separadores.begin();
	for ( ; it != separadores.end() ; it++) {
		this->tablero->remove(*(*it));
	}
}	     


//XXX: OJO QUE X E Y DEL GET_POINTER  ESTAN INTERCAMBIADOS, X REFIERE A "longitud", ancho, e Y a "latitud" o largo. un (8,2) para gtk es para mi (2,8)
bool Tablero::on_click_tablero(GdkEventButton* event){
	int x,y;
	int fila,columna;
	this->tablero->get_pointer(x,y); fila = (y-OFFSET)/SIZE;
	columna = (x-OFFSET)/SIZE;
	if (ultFilClick==fila && ultColClick==columna){
		return false;
		//No hago nada
	}else{
		celdaInteres = new Celda(fila,columna);
		this->matrizCeldas[fila][columna]=celdaInteres;
		m_signal_uncheck.emit();
	}
	ultFilClick=fila;
	ultColClick=columna;
	cambiarButons();
	return true;
}


void Tablero::actualizarMatriz(int x, int y){
	matrizCeldas.resize(x);
	for ( int i = 0 ; i < x ; i++ ) {
		matrizCeldas[i].resize(y);
	}
	for ( int i = 0 ; i < x ; i++){
		for ( int j = 0 ; j < y ; j++ ){
			matrizCeldas[i][j] = new Celda(i,j);
		}
	}
}

void Tablero::on_adj_changed_tablero(Gtk::SpinButton* spinbutton,int id){
	if (!celdaInteres)
		return;
	this->celdaInteres->on_adj_changed(spinbutton, id);
	butonsCambiados.push_back(spinbutton);
}

void Tablero::cambiarButons(){
	for ( unsigned int i = 0 ; i < butonsCambiados.size() ; i++ ){
		butonsCambiados[i]->set_value(0.00);
	}
	butonsCambiados.clear();
}

void Tablero::on_click_boton_tablero(int id){
	colInteres = columnas[id];
}

void Tablero::on_adjCols_changed_tablero(Gtk::SpinButton* spinbutton, int id){
	if (!colInteres)
		return;
	this->colInteres->on_adj_changed(spinbutton,id);	
}

//void Tablero::getProbColumnas(){
//	 for (unsigned int i = 0 ; i < columnas.size() ; i++ ) {
//		 std::cout << "col " << i<<" \t";
//		 for ( int j = 0 ; j < 16 ; j++ ) {
//			std::cout << columnas[i]->getInfo()->getProb_piezas(j)<< "," ;
//		 }
//		 std::cout<<std::endl;
//	 }
//}

//void Tablero::getProbCeldas(){
//	for (unsigned  int i = 0 ; i < matrizCeldas.size() ; i++ ) {
//		for ( unsigned int j = 0 ; j < matrizCeldas[0].size() ; j++ ){
//			for ( int k = 0 ; k < 16 ; k ++ ){
//				if ( matrizCeldas[i][j]==NULL)
//					std::cout<< "NULLLL " << std::endl;
//				std::cout << matrizCeldas[i][j]->getInfo()->getProb_piezas(j)<< "," ;
//			}
//			std::cout<< " " ;
//		}
//		std::cout<<std::endl;
//	}
//}

void Tablero::jsonColumnas(Json::Value& nivel,const std::string& nombre){
	std::stringstream ss;
	for ( unsigned i = 0 ; i < columnas.size() ; i++ ) {
		Json::Value aux(Json::arrayValue);
		for ( int j = 0 ; j < 16 ; j++ ){
			 aux.append(columnas[i]->getInfo()->getProb_piezas(j));
		}
		ss<<i ;
		nivel[nombre]["columnas"][ss.str()] = aux;
		ss.str("");
	}
}

void Tablero::jsonCeldas(Json::Value& nivel,const std::string& nombre){
	std::stringstream streamFila;
	std::stringstream streamColumna;
	for (unsigned  int i = 0 ; i < matrizCeldas.size() ; i++ ) {
		streamFila << i ;
		for ( unsigned int j = 0 ; j < matrizCeldas[0].size() ; j++ ){
			streamColumna << j ;
			if ( matrizCeldas[i][j]->isHueco() ){
				nivel[nombre]["celdas"][streamFila.str()][streamColumna.str()]["probabilidades"] = -1;
			}else{
				Json::Value aux(Json::arrayValue);
				for ( int k = 0 ; k < 16 ; k ++ ){
					aux.append(matrizCeldas[i][j]->getInfo()->getProb_piezas(k));
				}
				nivel[nombre]["celdas"][streamFila.str()][streamColumna.str()]["probabilidades"] = aux;
			}
			nivel[nombre]["celdas"][streamFila.str()][streamColumna.str()]["fondo"] = matrizCeldas[i][j]->getImage();
			streamColumna.str("");
		}
		streamFila.str("");
	}
}


void Tablero::on_image_changed_tablero(Gtk::FileChooser* fileChooser){
	std::cout<<fileChooser->get_filename() <<std::endl;
	this->celdaInteres->setImage(fileChooser->get_filename());
	Gtk::Image* img = new Gtk::Image(fileChooser->get_filename());
	img->set_size_request(SIZE,SIZE);
	this->tablero->put(*img,this->celdaInteres->getY()*SIZE+OFFSET,this->celdaInteres->getX()*SIZE+OFFSET);
	this->tablero->show_all();
}

void Tablero::on_check_button_tablero(){
	if (this->celdaInteres)
		this->celdaInteres->setHueco();
}


#include "tablero.h"

#include <gtkmm.h>

#define MAX_COLS 14
#define MIN_COLS 8
#define MIN_FILAS 8
#define MAX_FILAS 14
#define SIZE 40
#define SIZE_CELDA 10
#define OFFSET 20


Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("f_tablero",tablero);
	builder->get_widget("event_tablero",eventos_tablero);
	this->eventos_tablero->set_events(Gdk::BUTTON_PRESS_MASK);
	this->eventos_tablero->signal_button_press_event().connect(sigc::mem_fun(*this,&Tablero::on_click_tablero));
	cantFilas=MIN_FILAS;
	cantColumnas=MIN_COLS;
	//this->setter=aSetter;
}

Tablero::~Tablero(){}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	int X = spin_x->get_value_as_int() + 1;
	if 	( X > cantFilas ){
		agregarFilas(X);
	}else if( X < cantFilas ){
		borrarSeps();
	}
	cantFilas=X;
	actualizarMatriz(cantFilas,cantColumnas);
}

void Tablero::on_cordy_changed(Gtk::SpinButton* spin_y){
	int Y = spin_y->get_value_as_int() + 1;
	if ( Y > cantColumnas ) {
		agregarColumnas(Y);
	}else if ( Y < cantColumnas ) {
		borrarSeps();
	}
	cantColumnas=Y;
	actualizarMatriz(cantFilas,cantColumnas);
}

void Tablero::agregarFilas(int X){
	for ( int i= 0 ; i < X ; i++ ){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*X,SIZE);
		this->tablero->put(*sep_horizontal,0,i*SIZE);
		this->tablero->show_all();
	} 
}

void Tablero::agregarColumnas(int Y){
	for ( int i = 0 ; i < Y ; i++ ){
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*Y);
		this->tablero->put(*sep_vertical,i*SIZE,0);
		this->tablero->show_all();
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
	this->tablero->get_pointer(x,y);
	fila = (y-OFFSET)/SIZE;
	columna = (x-OFFSET)/SIZE;
	if (ultFilClick==fila && ultColClick==columna){
		return false;
		//No hago nada
	}else{
		celdaInteres = new Celda(fila,columna);
		//this->setter->completar(celdaInteres);
		this->matrizCeldas[fila][columna]=celdaInteres;
	}
	ultFilClick=fila;
	ultColClick=columna;

	std::cout << "estoy en : " << (int)((y-OFFSET)/SIZE) << " , " << (int)((x-OFFSET)/SIZE)<< std::endl;
	//std::cout << "tengo:  " <<  << " , " << (int)y/SIZE << std::endl;
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


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
	celdaInteres=NULL;
	builder->get_widget("f_tablero",tablero);
	builder->get_widget("event_tablero",eventos_tablero);
	this->eventos_tablero->set_events(Gdk::BUTTON_PRESS_MASK);
	cantFilas=MIN_FILAS;
	//agregarFilas(cantFilas);
	cantColumnas=MIN_COLS;
	//agregarColumnas(cantColumnas);
	//this->tablero->show_all();
	ultFilClick=MAX_FILAS;
	ultColClick=MAX_COLS;
	actualizarMatriz(cantFilas,cantColumnas);
	this->eventos_tablero->signal_button_press_event().connect(sigc::mem_fun(*this,&Tablero::on_click_tablero));
	
}

Tablero::~Tablero(){}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	int X = spin_x->get_value_as_int() + 1;
	if 	( X > cantFilas ){
		cantFilas=X;
		agregarFilas(X);
		alargarColumnas(X);
	}else if( X < cantFilas ){
		borrarSeps();
	}
	cantFilas=X;
	actualizarMatriz(cantFilas,cantColumnas);
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
	actualizarMatriz(cantFilas,cantColumnas);
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
	}
	ultFilClick=fila;
	ultColClick=columna;

	std::cout << "estoy en : " << (int)((y-OFFSET)/SIZE) << " , " << (int)((x-OFFSET)/SIZE)<< std::endl;
	//std::cout << "tengo:  " <<  << " , " << (int)y/SIZE << std::endl;
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
	this->colInteres->on_adj_changed(spinbutton,id);	
}

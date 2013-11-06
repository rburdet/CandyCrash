#include "tablero.h"

#include <gtkmm.h>

#define MAX_COLS 14
#define MAX_FILAS 14
#define SIZE 40
#define SIZE_CELDA 10


Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("f_tablero",tablero);
	builder->get_widget("event_tablero",eventos_tablero);
	this->eventos_tablero->set_events(Gdk::BUTTON_PRESS_MASK);
	this->eventos_tablero->signal_button_press_event().connect(sigc::mem_fun(*this,&Tablero::on_click_tablero));
	//alto = tablero->get_height();
	//ancho = tablero->get_width();
	cantFilas=0;
	cantColumnas=0;
}

Tablero::~Tablero(){}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	//std::cout<< "cambio: " << spin_x->get_value_as_int() << std::endl;
	int X = spin_x->get_value_as_int() + 1;
	if 	( X > cantFilas ){
		agregarFilas(X);
	}else if( X < cantFilas ){
		borrarSeps();
	}
	cantFilas=X;
}

void Tablero::on_cordy_changed(Gtk::SpinButton* spin_y){
	int Y = spin_y->get_value_as_int() + 1;
	if ( Y > cantColumnas ) {
		agregarColumnas(Y);
	}else if ( Y < cantColumnas ) {
		borrarSeps();
	}
}

void Tablero::agregarFilas(int X){
	for ( int i= 0 ; i < X ; i++ ){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*X,SIZE_CELDA);
		this->tablero->put(*sep_horizontal,0,i*SIZE);
		this->tablero->show_all();
	} 
}

void Tablero::agregarColumnas(int Y){
	for ( int i = 0 ; i < Y ; i++ ){
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE_CELDA,SIZE*Y);
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


bool Tablero::on_click_tablero(GdkEventButton* event){
	std::cout<<"CLICKKK"<<std::endl;
	return true;
}


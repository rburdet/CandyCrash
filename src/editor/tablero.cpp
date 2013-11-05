#include "tablero.h"

#define MAX_COLS 14
#define MAX_FILAS 14
#define SIZE 40
#define SIZE_CELDA 10


Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("f_tablero",tablero);
	alto = tablero->get_height();
	ancho = tablero->get_width();
	cantFilas=0;
	cantColumnas=0;
}

Tablero::~Tablero(){}
//Gtk::Fixed* Tablero::getTablero(){
//	return this->tablero;
//}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	//std::cout<< "cambio: " << spin_x->get_value_as_int() << std::endl;
	int X = spin_x->get_value_as_int();
	if 	( X > cantFilas ){
		agregarFilas(X);
	}else if( X < cantFilas ){
		borrarFilas(X);
	}
	cantFilas=X;
}

void Tablero::on_cordy_changed(Gtk::SpinButton* spin_y){
	int Y = spin_y->get_value_as_int();
	if ( Y > cantColumnas ) {
		agregarColumnas(Y);
	}else if ( Y < cantColumnas ) {
		borrarColumnas(Y);
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

//TODO: NO ESTA BORRANDO LAS LINITAS
void Tablero::borrarFilas(int X){
	//std::cout<<"BORRAR"<<std::endl;
	//std::vector<Gtk::Widget*> separadores = this->tablero->get_children();
	//for ( int i=cantFilas ; i < X ; i++ ){
	//	this->tablero->remove(*separadores.at(i));
	//}
}

void Tablero::borrarColumnas(int Y){
	//std::cout<<"BORRAR"<<std::endl;
	//std::vector<Gtk::Widget*> separadores = this->tablero->get_children();
	//for ( int i=cantColumnas ; i < Y ; i++ ){
	//	this->tablero->remove(*separadores.at(i));
	//}
}

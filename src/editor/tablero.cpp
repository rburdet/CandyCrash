#include "tablero.h"

Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("f_tablero",tablero);
}

Tablero::~Tablero(){}
//Gtk::Fixed* Tablero::getTablero(){
//	return this->tablero;
//}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	std::cout<< "cambio: " << spin_x->get_value_as_int() << std::endl;
}


void Tablero::on_cordy_changed(Gtk::SpinButton* spin_y){
	std::cout<< "cambio: " << spin_y->get_value_as_int() << std::endl;
}



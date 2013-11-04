#include "tablero.h"

Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("f_tablero",tablero);
}

//Gtk::Fixed* Tablero::getTablero(){
//	return this->tablero;
//}

void Tablero::on_cordx_changed(){
	std::cout<< "cambio: " << std::endl;
}

void Tablero::on_cordy_changed(){

}


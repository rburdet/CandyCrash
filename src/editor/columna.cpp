#include "columna.h"

Columna::Columna(int x,Gtk::Button* button){
	this->pos=x;
	this->boton=button;
}

Columna::~Columna(){
}

void Columna::on_adj_changed(Gtk::SpinButton* spinbutton,int id){
	prob_piezas[id]=spinbutton->get_value_as_int();
}

Gtk::Button* Columna::get_boton(){
	return boton;
}

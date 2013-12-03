#include "columna.h"

Columna::Columna(int x,Gtk::Button* button){
	this->pos=x;
	this->boton=button;
	this->probs = new Info();
}

Columna::~Columna(){
	delete this->probs;
}

void Columna::on_adj_changed(Gtk::SpinButton* spinbutton,int id){
	this->probs->setProb_piezas(spinbutton->get_value_as_int(),id);
}

Gtk::Button* Columna::get_boton(){
	return boton;
}

Info* Columna::getInfo(){
	return this->probs;
}


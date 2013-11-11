#include "celda.h"

#include <iostream>


Celda::Celda(int x, int y){
	this->x = x;
	this->y = y;
	this->probs = new Info();
}

int Celda::getX(){
	return this->x;
}

int Celda::getY(){
	return this->y;
}

void Celda::on_adj_changed(Gtk::SpinButton * spinbutton, int id){
	probs->setProb_piezas(spinbutton->get_value_as_int(),id);
}

Info* Celda::getInfo(){
	return this->probs;
}

Celda::~Celda(){
	delete this->probs;
}

void Celda::setImage(const std::string& fileName){
	this->imgPath=fileName;
}

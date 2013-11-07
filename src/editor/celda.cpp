#include "celda.h"

#include <iostream>

Celda::Celda(int x, int y){
	this->x = x;
	this->y = y;
	std::cout << x << " , " << y << std::endl;
}

int Celda::getX(){
	return this->x;
}

int Celda::getY(){
	return this->y;
}

void Celda::on_adj21_changed(Gtk::SpinButton* spinbutton21){
	std::cout<< spinbutton21->get_value_as_int()<< std::endl;
}

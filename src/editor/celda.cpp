#include "celda.h"

#include <iostream>


Celda::Celda(int x, int y){
	this->inicializarVector();
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

void Celda::inicializarVector(){
	for ( int i = 0 ; i < NUMEROPIEZAS ; i++ ) {
		prob_piezas[i] = 0 ; 
	}
}

void Celda::on_adj_changed(Gtk::SpinButton * spinbutton, int id){
	prob_piezas[id]=spinbutton->get_value_as_int();
}


#include "common.caramelo.h"

#define DEFAULTIMAGEDIR "../Imagenes/"
#define SIZE 40

Caramelo::Caramelo(int idCaramelo, const std::string& imgDir,int i, int j) : image(imgDir) {
	this->x = i;
	this->y = j;
	this->idCaramelo = idCaramelo;
	this->set_relief(Gtk::RELIEF_NONE);
	this->image.set_size_request(SIZE,SIZE);
	this->set_image(image);
	this->show();
}

int Caramelo::getX(){
	return x;
}

int Caramelo::getY(){
	return y;
}

void Caramelo::setX(int x){
	this->x = x;
}

void Caramelo::setY(int y){
	this->y = y ;
}

void Caramelo::opacar(){
	double aux = this->get_opacity() - 0.01;
	set_opacity(aux);
}

void Caramelo::hacerAparecer(){
	double aux = this->get_opacity() + 0.01;
	set_opacity(aux);
}

bool Caramelo::visible(){
	return (this->get_opacity()!=0);
}

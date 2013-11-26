#include "common.caramelo.h"

#define DEFAULTIMAGEDIR "../Imagenes/"
#define SIZE 40

Caramelo::Caramelo(int idCaramelo, const std::string& imgDir,int i, int j) : image(imgDir) {
	this->moviendo = false;
	this->x = i;
	this->y = j;
	this->x_pos = i * SIZE + 20;
	this->y_pos = j * SIZE + 20;
	this->idCaramelo = idCaramelo;
	this->set_relief(Gtk::RELIEF_NONE);
	this->image.set_size_request(SIZE,SIZE);
	this->set_image(image);
	this->show();
}

int Caramelo::getX(){
	return x;
}

int Caramelo::getXPos(){
	return x_pos;
}

int Caramelo::getY(){
	return y;
}

int Caramelo::getYPos(){
	return y_pos;
}

void Caramelo::setX(int x){
	this->x = x;
}

void Caramelo::setXPos(int x){
	this->x_pos = x;
}

void Caramelo::setY(int y){
	this->y = y ;
}

void Caramelo::setYPos(int y){
	this->y_pos = y ;
}

bool Caramelo::mover(int x, int y){
	if(this->x_mov == x && this->y_mov == y)
		return false;

	return this->mover();
}
bool Caramelo::mover(){
	return this->moviendo;
}

bool Caramelo::setMoviendo(bool moviendo, int x, int y){
	this->setMoviendo(moviendo);
	this->x_mov = x;
	this->y_mov = y;
}
bool Caramelo::setMoviendo(bool moviendo){
	this->moviendo = moviendo;
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

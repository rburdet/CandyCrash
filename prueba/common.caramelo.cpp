#include "common.caramelo.h"

#define DEFAULTIMAGEDIR "../Imagenes/"
#define SIZE 40

Caramelo::Caramelo(int idCaramelo,const std::string& imageName) : image(imageName) {
	this->idCaramelo = idCaramelo;
	this->show_all();
	this->set_relief(Gtk::RELIEF_NONE);
	image.set_size_request(SIZE,SIZE);
	this->set_image(image);
}


#include "common.caramelo.h"

#define DEFAULTIMAGEDIR "../Imagenes/"
#define SIZE 40

Caramelo::Caramelo(int idCaramelo, const std::string& imgDir) : image(imgDir) {
	this->idCaramelo = idCaramelo;
	this->set_relief(Gtk::RELIEF_NONE);
	this->image.set_size_request(SIZE,SIZE);
	this->set_image(image);
	this->show();
}


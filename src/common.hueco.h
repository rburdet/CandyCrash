#ifndef HUECO_H
#define HUECO_H

#include <gtkmm.h>

#define HUECODIR "../imagenes/hueco.png"

class Hueco : public Gtk::Image{
	public:
		Hueco() : (std::string(HUECODIR)){}
		~Hueco(){}
};

#endif

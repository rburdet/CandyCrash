#ifndef CARAMELO_H
#define CARAMELO_H

#include <gtkmm.h>
#include "common.caramelos.h"

class Caramelo : public Gtk::Button{
	public:
		Caramelo(int idCaramelo, const std::string& imageName);
		virtual ~Caramelo(){};
		virtual bool mover()=0;
		void hablar(){ std::cout << idCaramelo << std::endl; }
		//virtual void mostrar()=0;
	private:
		int idCaramelo;
		Gtk::Image image;
};

#endif

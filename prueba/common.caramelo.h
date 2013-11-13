#ifndef CARAMELO_H
#define CARAMELO_H

#include <gtkmm.h>
#include "common.caramelos.h"

class Caramelo : public Gtk::Image{
	public:
		Caramelo(int idCaramelo);
		virtual ~Caramelo(){};
		virtual bool mover()=0;
		//virtual void mostrar()=0;
	private:
		int idCaramelo;
};

#endif

#ifndef CARAMELO_H
#define CARAMELO_H

#include <gtkmm.h>
#include "common.caramelos.h"
#include <iostream>

class Caramelo : public Gtk::Button{
	public:
		Caramelo(int idCaramelo, const std::string& imageName,int i, int j);
		virtual ~Caramelo(){};
		virtual bool mover()=0;
		void hablar(){ std::cout << idCaramelo << std::endl; }
		void opacar();
		bool visible();
		void hacerAparecer();
		//virtual void mostrar()=0;
		int getX();
		int getY();
		void setX(int x);
		void setY(int y);
	private:
		int x;
		int y;
		int idCaramelo;
		Gtk::Image image;
};

#endif

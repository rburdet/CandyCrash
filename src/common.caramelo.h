#ifndef CARAMELO_H
#define CARAMELO_H

#include <gtkmm.h>
#include "common.caramelos.h"
#include <iostream>

class Caramelo : public Gtk::Button{
	public:
		Caramelo(int idCaramelo, const std::string& imageName,int i, int j);
		virtual ~Caramelo(){};
		/** Chequea si el caramelo esta en movimiento.
		 * Si esta en movimiento, pero se pasan como parametros las coordenadas finales, devolvera false.
		 * @param x: coordenada final x
		 * @param y: coordeanda final y
		 * @return verdadero si esta en movmiento, falso si no
		 */
		bool mover(int x, int y);
		bool mover();
		bool setMoviendo(bool moviendo, int x, int y);
		bool setMoviendo(bool moviendo);

		void hablar(){ std::cout << idCaramelo << std::endl; }
		int getId(){ return this->idCaramelo; }
		void opacar();
		bool visible();
		void hacerAparecer();
		//virtual void mostrar()=0;
		int getX();
		int getXPos();
		int getY();
		int getYPos();
		void setX(int x);
		void setXPos(int x);
		void setY(int y);
		void setYPos(int y);
	private:
		int x;
		int y;
		int x_pos;
		int y_pos;
		int idCaramelo;
		Gtk::Image image;
		bool moviendo;
		int x_mov;
		int y_mov;
};

#endif

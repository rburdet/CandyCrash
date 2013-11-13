#ifndef DIBUJARDOR_H
#define DIBUJADOR_H

#include <iostream>
#include <vector>
#include <gtkmm.h>
#include <jsoncpp/json/json.h>
#include "cliente.window.h"
#include "common.factory_caramelos.h"

#define MAXELEMENTOS 16
#define SIZE 50
#define SIZEIMAGEN 40

class TableroJuego : public Window {
	public:
		TableroJuego(Json::Value mapa);
		~TableroJuego(){}
		void dibujar(){}
		virtual void mensaje(Json::Value& data){}

	private:
		Gtk::Fixed tablero;
		int dimX;
		int dimY;
		Json::Value mapa;
		std::string nMapa;

		std::vector<std::vector<Gtk::Image*> > matrizFondos;
		std::vector<std::vector<Caramelo*> > matrizCaramelos;

		void crearMatrices();
		int getX();
		int getY();
		std::string getNMapa();
		void dibujarLineas();
		void llenar();
};

#endif

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
		static std::string getNMapa(Json::Value mapa);
		int clicks;

	private:
		Gtk::Fixed tablero;
		Gtk::AspectFrame frameTablero;
		int dimX;
		int dimY;
		Json::Value mapa;
		std::string nMapa;

		std::vector<std::vector<Gtk::Image*> > matrizFondos;
		std::vector<std::vector<Caramelo*> > matrizCaramelos;
		int old_x;
		int old_y;
		int step1;
		int step2;

		void crearMatrices();
		int getX();
		int getY();
		void dibujarLineas();
		void llenar();
		void conectarCaramelos();
		void mover(int x , int y);
		void mover2Piezas(int pos1, int pos2);
		bool onTimeout(int x,int y);
		sigc::connection conTimeout; 
};

#endif

#ifndef DIBUJARDOR_H
#define DIBUJADOR_H

#include <iostream>
#include <vector>
#include <gtkmm.h>
#include <glibmm/signalproxy.h>
#include <jsoncpp/json/json.h>
#include "cliente.window.h"
#include "common.factory_caramelos.h"

#define MAXELEMENTOS 16
#define SIZE 50
#define SIZEIMAGEN 40

typedef enum {
	VERTICAL=0,
	HORIZONTAL=1
} Direccion;

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
		std::vector<std::vector<sigc::connection> > m_senal;
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
		void desconectarCaramelos();
		void mover(int x , int y);
		void mover2Piezas(int pos1, int pos2, int DIRECCION);
		bool onTimeout(int x,int y,int DIRECCION);
		sigc::connection conTimeout; 
};

#endif

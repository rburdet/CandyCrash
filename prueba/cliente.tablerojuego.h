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
	ARRIBA=0,
	ABAJO=1,
	DERECHA,
	IZQUIERDA
} Direccion;

class TableroJuego : public Window {
	public:
		TableroJuego(Json::Value mapa);
		~TableroJuego(){}
		void dibujar(){}
		virtual void mensaje(Json::Value& data){}
		static std::string getNMapa(Json::Value mapa);

	private:
		Gtk::Fixed tablero;
		Gtk::AspectFrame frameTablero;
		int dimX;
		int dimY;
		Json::Value mapa;
		std::string nMapa;

		std::vector<std::vector<Gtk::Image*> > matrizFondos;
		std::vector<std::vector<Caramelo*> > matrizCaramelos;

		int clicks;
		int originX;
		int originY;
		int step1;
		int step2;
		Caramelo* carameloOrigen;

		void crearMatrices();
		int getX();
		int getY();
		void dibujarLineas();
		void llenar();
		void conectarCaramelos();
		void click(Caramelo* caramelo);
		void mover2Piezas(int pos1, int pos2, int DIRECCION);
		bool onTimeout(int x,int y,int DIRECCION);
		bool swapBoton(Caramelo* Origen, Caramelo* Final,int DIRECCION);
		void swapMatriz (Caramelo* Origen,Caramelo* Final);
		sigc::connection conTimeout; 
};

#endif

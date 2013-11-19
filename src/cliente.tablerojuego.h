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
	DERECHA=1,
	ABAJO=2,
	IZQUIERDA
} Direccion;

class TableroJuego : public Window {
	public:
		TableroJuego(Json::Value mapa);
		~TableroJuego(){}
		void dibujar(){}
		virtual void mensaje(Json::Value& data);

	private:
		Gtk::Fixed tablero;
		Gtk::AspectFrame frameTablero;
		int dimX;
		int dimY;
		Json::Value mapa;
		std::string nMapa;

		std::vector<std::vector<Gtk::Image*> > matrizFondos;
		std::vector<std::vector<Caramelo*> > matrizCaramelos;
		std::vector<std::vector<Caramelo*> > matrizCaramelosAux;

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
		void mover2Piezas(int pos1, int pos2, int DIRECCION, bool volver);
		bool onTimeout(int x,int y,int DIRECCION,bool volver);
		bool swapBoton(Caramelo* Origen, Caramelo* Final,int DIRECCION);
		sigc::connection conTimeout; 

		/** Efectua cada movimiento
		 */
		void onMovimiento(Json::Value& data);

		/** Mueve un caramelo a posiciones de tablero
		 * @param car: caramelo a mover
		 * @param xF: posicion final de tablero en x (casillero)
		 * @param yF: posicion final de tablero en y (casillero)
		 */
		void moverPieza(Caramelo* car, int xF, int yF);

		/** Funcion que se usa con timeout para crear movimientos.
		 * @param car: caramelo a mover
		 * @param x_final: posicion final x del caramelo (coordenadas)
		 * @param y_final: posicion final y del caramelo (coordenadas)
		 * @param step_x: salto que pega en x por cada iteracion
		 * @param step_y: salto que pega en y por cada iteracion
		 * @return true si no termino, false si llego a destino
		 */
		bool animationMove(Caramelo* car, int x_final, int y_final, int step_x, int step_y);
};

#endif

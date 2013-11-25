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

		/** Metodo que se usa para recibir mensajes.
		 * @param data: mensaje { "event": , ... }
		 */
		virtual void mensaje(Json::Value& data);

	private:
		Gtk::Fixed tablero;
		Gtk::AspectFrame frameTablero;
		//DEPRECATED:
		int dimX;
		int dimY;
		//Ahora se usa esto:
		int alto;
		int ancho;
		Json::Value mapa;
		std::string nMapa;

		// TODO: meow meowww meow, es decir, no se esta usando,, jaja
		std::vector<std::vector<Gtk::Image*> > matrizFondos;
		// matrizCaramelos[y][x]
		std::vector<std::vector<Caramelo*> > matrizCaramelos;
		// matrizCaramelosAux[y][x]
		std::vector<std::vector<Caramelo*> > matrizCaramelosAux;

		int clicks;
		int colOrigen;
		int filaOrigen;
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
		bool asomar(Caramelo* Origen, Caramelo* Final,int DIRECCION);
		sigc::connection conTimeout; 

		/** Efectua cada movimiento.
		 * @param data: estructura de movimiento en Json { "x": , "y", "mov": (CaramelosMovimientos), } puede tener un miembro opcional "car" cuando es el movimiento de caramelo nuevo.
		 */
		void onMovimiento(Json::Value& data);

		/** Mueve un caramelo a posiciones de tablero.
		 * Llama a animationMove con un timeout, se usan numeros hardcodeados para los steps y para el tiempo del timeout.
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

		void moveCaramelo(int x, int y, int xf, int yf);
};

#endif

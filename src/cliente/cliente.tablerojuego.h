#ifndef DIBUJARDOR_H
#define DIBUJADOR_H

#include <iostream>
#include <vector>
#include <gtkmm.h>
#include <string>
#include <glibmm/signalproxy.h>
#include <jsoncpp/json/json.h>
#include "cliente.window.h"
#include "cliente.factory_caramelos.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

#define MAXELEMENTOS 16
#define SIZE 50
#define SIZEIMAGEN 40

//typedef enum {
//	ARRIBA=0,
//	DERECHA=1,
//	ABAJO=2,
//	IZQUIERDA
//} Direccion;

class TableroJuego : public Window {
	public:
		explicit TableroJuego(Json::Value mapa);
		~TableroJuego(){}
		void dibujar(){}

		/** Metodo que se usa para recibir mensajes.
		 * @param data: mensaje { "event": , ... }
		 */
		virtual void mensaje(Json::Value& data);

	private:
		Gtk::Fixed tablero;
		Gtk::Image imagenFondo;
		Gtk::AspectFrame frameTablero;
		int dimX;
		int dimY;
		
		Json::Value mapa;
		std::string nMapa;
		std::string sonidoMov;
		std::string sonidoDestruir;

		// TODO: meow meowww meow, es decir, no se esta usando,, jaja
		std::vector<std::vector<Gtk::Image*> > matrizFondos;
		/** matrizCaramelos[x][y]
		 */
		std::vector<std::vector<Caramelo*> > matrizCaramelos;
		/** matrizCaramelosAux[x][y]
		 */
		std::vector<std::vector<Caramelo*> > matrizCaramelosAux;
		std::vector<Json::Value> movimientos;
		int movimientosCount;

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
		//void mover2Piezas(int pos1, int pos2, int DIRECCION, bool volver);
		//bool onTimeout(int x,int y,int DIRECCION,bool volver);
		//bool swapBoton(Caramelo* Origen, Caramelo* Final,int DIRECCION);
		bool asomar(Caramelo* Origen, Caramelo* Final,int DIRECCION);
		sigc::connection conTimeout; 
		void aparecer(Caramelo* caramelo);
		bool onAclarar(Caramelo* caramelo);

		bool onOpacar(Caramelo* caramelo);
		void esfumar(Caramelo* caramelo);

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
		bool animationMove(Caramelo* car, int x_final, int y_final, int step_x,
				int step_y);

		void moveCaramelo(int x, int y, int xf, int yf);

		void triggerMovimientos();

		bool onClose();
};

#endif

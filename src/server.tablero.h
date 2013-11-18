#ifndef __SERVER_TABLER_H__
#define __SERVER_TABLER_H__

#include <string>
#include <jsoncpp/json/json.h>
#include "common.caramelos.h"
#include "common.caramelos_movimientos.h"

class Tablero {
	public:
		Tablero(std::string path);
		~Tablero();

		/** Ejecuta un movimiento.
		 * @param x: posicion x de la ficha
		 * @param y: posicion y de la ficha
		 * @param mov: para donde es el movimiento
		 * @param movimientos[out]: JSON, lista de los movimientos que se realizaron.
		 * @return devuelve la cantidad de puntos que gano el usuario o 0 si no se realizo el movimiento
		 */
		int movimiento(int x, int y, CaramelosMovimientos mov, Json::Value& movimientos);
		Json::Value getTablero();

	protected:
		Json::Value mapa;
		Json::Value tablero;
		Json::Value probabilidades;
		int dim_x;
		int dim_y;
		void generar();
		void efectivizarCelda(Json::Value& celda);
		void getMax(double* arr,double& max, int& pos);

		/** Chequea si es un movimiento valido, es decir si realizo alguna convinacion
		 */
		bool movimientoValido(int x, int y, CaramelosMovimientos mov);

		/** Calcula las nuevas coordenadas dsp del movimiento
		 */
		void calcularCoordenadas(int x, int y, CaramelosMovimientos mov, int&xf, int &yf);

		/** Efectua accion de intercambio con star.
		 */
		int doStar(Caramelos carameloMovido, int x, int y, Caramelos carameloMovido2, int xf, int yf, Json::Value& movimientos);

		/** Calcula si las dos fichas son del mismo color.
		 * @param car: caramelo
		 * @param car: otro caramelo
		 * @return verdadero si son del mismo color, falso si no lo son
		 */
		bool esMismoColor(Caramelos car, Caramelos car2);

		/** Transforma un numero a string.
		 * @param n: numero a transformar.
		 * @return string que representa ese numero
		 */
		std::string num2str(int n);

		/** Crea la estructura de un movimiento para ser enviado.
		 * @param x: posicion en x del caramelo.
		 * @param y: posicion en y del caramelo
		 * @param mov: movimiento que realizo el caramelo.
		 * @return la estructura de movimiento para ser enviada en formato JSON.
		 */
		Json::Value newMov(int x, int y, CaramelosMovimientos mov);

		/** Crea la estructura de un movimiento para ser enviado.
		 * Este metodo se usa para cuando aparecen caramelos
		 * @param x: posicion en x del caramelo.
		 * @param y: posicion en y del caramelo
		 * @param mov: movimiento que realizo el caramelo.
		 * @param car: caramelo
		 * @return la estructura de movimiento para ser enviada en formato JSON.
		 */
		Json::Value newMov(int x, int y, CaramelosMovimientos mov, Caramelos car);

		/** Invierte el movimiento pasado.
		 * @param mov: movimiento a invertir.
		 * @return movimiento invertido.
		 */
		CaramelosMovimientos movInverso(CaramelosMovimientos mov);

		/** Rellena los rellenar, haciendo la caida de los caramelos de mas arriba
		 * @param movimientos[ou]: movimientos de caramelos que re realizados.
		 */
		void rellenarTablero(Json::Value & movimientos);

		/** Busca si hay patrones y los reemplaza por RELLENAR, sumandole a puntos los puntos ganados.
		 * Puntos debe estar inicializado previamente en algun valor.
		 * @param movimientos[out]: movimientos que se realizan
		 * @param puntos[out]: puntos ganados
		 * @return true si hubo patrones, false si no.
		 */
		bool hayPatrones(Json::Value & movimientos, int & puntos);

		/** Devuelve true si es button
		 */
		bool esButton(Caramelos car);

		/** Activa una combinacion de columna encontrada.
		 * @param x[in]: 
		 * @param y:
		 * @param todosButtons: verdadero si la combinacion es de todos buttons
		 * @param movimientos[out]: array que contiene los movimientos que se realizaron
		 * @param puntos[out]: se le suman los puntos que se ganaron por activar
		 * @param x_mov: posicion x donde se colocara el caramelo especial de tenerse qe generar
		 * @param y_mov: posicion y donde se colocara el caramelo especial de tenerse que generar
		 * @return verdadero si se realizo una combinacion
		 */
		bool activarCombinacionColumna(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos, int x_mov, int y_mov);

		/** Ver la otra declaracion.
		 * Aca se pasan como x_mov = x ; y_mov = y - contador/2;
		 */
		bool activarCombinacionColumna(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos);

		/** Activa una combinacion de fila encontrada.
		 * @param x[in]: 
		 * @param y:
		 * @param todosButtons: verdadero si la combinacion es de todos buttons
		 * @param movimientos[out]: array que contiene los movimientos que se realizaron
		 * @param puntos[out]: se le suman los puntos que se ganaron por activar
		 * @param x_mov: posicion x donde se colocara el caramelo especial de tenerse qe generar
		 * @param y_mov: posicion y donde se colocara el caramelo especial de tenerse que generar
		 * @return verdadero si se realizo una combinacion
		 */
		bool activarCombinacionFila(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos, int x_mov, int y_mov);

		/** Ver la otra declaracion.
		 * Aca se pasan como x_mov = x - contador/2; y_mov = y;
		 */
		bool activarCombinacionFila(int x, int y, bool todosButtons, int contador, Json::Value & movimientos, int &puntos);

		/** Metodo usado para cuando se dispara un bar (para efectuar sobre toda la columna)
		 * @param x: posicion x de la columna
		 * @param movimientos[out]: lista de movimientos realizados
		 * @param puntosx: puntos por cada caramelo sacado
		 * @param puntos[out]: se le suman todos los puntos ganados
		 */
		void dispararColumna(int x, Json::Value& movimientos, int puntosx, int& puntos);

		/** Metodo usado para cuando se dispara un bar (para efectuar sobre toda la fila)
		 * @param y: posicion y de la fila
		 * @param movimientos[out]: lista de movimientos realizados
		 * @param puntosx: puntos por cada caramelo sacado
		 * @param puntos[out]: se le suman todos los puntos ganados
		 */
		void dispararFila(int y, Json::Value& movimientos, int puntosx, int& puntos);

		/** Comprueba si hay y efectua la combinacion sobre una ficha.
		 * @param car: Caramelo de la ficha
		 * @param x: posicion x del caramelo
		 * @param y: posicion y del caramelo
		 * @param movimientos[out]: lista de movimientos realizados
		 * @return cantidad de puntos sumados
		 */
		int doCombinacion(Caramelos car, int x, int y, Json::Value& movimientos);

		/** Devuelve un bar vertical del color de car.
		 */
		Caramelos verBarColor(Caramelos car);

		/** Devuelve un bar horizontal del color de car.
		 */
		Caramelos horBarColor(Caramelos car);
		
};

#endif

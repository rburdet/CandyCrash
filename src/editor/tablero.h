#ifndef TABLERO_H
#define TABLERO_H


#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <glibmm/listhandle.h>
#include <gtkmm.h>
#include <sstream>
#include <string>
#include <vector>
#include "celda.h"
#include "columna.h"

#define COLUMNAS 1 
#define FILAS 2
/** Tablero de juego .
 */
class Tablero{
	public:
		/** Constructor de tablero.
		 * @param builder: Utilizado para levantar widgets del archivo de 
		 * glade
		 */
		explicit Tablero(Glib::RefPtr<Gtk::Builder>& builder);
		virtual ~Tablero();
		//Handlers accedidos desde la configuracion basica
		
		/** Se encarga de poner probabilidades por columna cuando
		 * un spinbutton cambia de valor.
		 * @param spinbutton: Spinbutton que cambio de valor.
		 * @param id: Numero de spinbutton que cambio de valor.
		 */
		void on_adjCols_changed_tablero(Gtk::SpinButton* spinbutton,int id);

		/** Guarda el valor de spin_x que contiene la cantidad de filas
		 * seleccionada.
		 */
		void on_cordx_changed(Gtk::SpinButton* spin_x);

		/** Guarda el valor de spin_y que contiene la cantidad de columnas
		 * seleccionada.
		 */
		void on_cordy_changed(Gtk::SpinButton* spin_y);

		/** Se encarga de poner probabilidades por celda.
		 * @param spinbutton: SpinButton que cambio.
		 * @param id: Numero de SpinButton que cambio.
		 */
		void on_adj_changed_tablero(Gtk::SpinButton* spinbutton,int id);

		/** Pone una imagen a una celda.
		 * @param fileChooser: FileChooser que contiene el archivo 
		 * seleccionado.
		 */
		void on_image_changed_tablero(Gtk::FileChooser* fileChooser);

		/** Pone una imagen de fondo al tablero.
		 * @param fileChooser: FileChooser que contiene el archivo 
		 * seleccionado.
		 */
		void on_image_fondo_changed_tablero(Gtk::FileChooser* fileChooser);

		/** Pone un hueco en la celda que este seleccionada.
		 */
		void on_check_button_tablero();

		//Metodos serializadores
		
		/** Serializa la informacion de las celdas.
		 */
		void jsonCeldas(Json::Value& nivel,const std::string& nombre);

		/** Serializa la informacion de las celdas.
		 */
		void jsonColumnas(Json::Value& nivel,const std::string& nombre);
		
		//Senal del que conectara el checkbutton.
		
		/** Senal que se emite despues de hacer click
		 */
		typedef sigc::signal< void > type_signal_uncheck;
		type_signal_uncheck signal_uncheck();

	private:
		//**************************************************//
		//					ATRIBUTOS						//
		//Elementos importantes al hacer un click
		Celda* celdaInteres;
		Columna* colInteres;
		//Matriz que maneja el tablero
		std::vector<std::vector<Celda*> > matrizCeldas;
		std::string imagenFondo;

		std::vector<std::vector<std::vector< int > > >matrizButons;
		//Columnas para la probabilidad seteada
		std::vector< Columna* > columnas;
		
		//Tablero con su respectivo event box (fue necesario para tomar los 
		//clicks hechos sobre el mismo)
		Gtk::Fixed* tablero;
		Gtk::EventBox* eventos_tablero;

		Gtk::Image* point;
		//Spinbuttons que cambiaron cada vez que se eligio una celda
		std::vector< Gtk::SpinButton* > butonsCambiados;
		std::vector< Gtk::SpinButton* > butonsColumnasCambiados;

		//Datos del tablero
		int cantColumnas;
		int cantFilas;

		//Para tener una referencia de lo ultimo que se toco y no volver a 
		//pedir informacion:
		int ultFilClick;
		int ultColClick;
		

		// ***************************************************************//
		//								METODOS							  //
		//Metodos propios 

		/** Crea matriz de botones para almacenar los valores de los buttons.
		 */ 
		void crearMatrizBotones(int x , int y);

		/** Agrega filas al tablero.
		 * @param X: Cantidad de filas a agregar.
		 */
		void agregarFilas(int X);
		/** Agrega columnas al tablero.
		 * @param X: Cantidad de columnas a agregar.
		 */
		void agregarColumnas(int Y);
		//Borra la cantidad de filas o columnas (lo que indique el segundo 
		//parametro) y todo el contenido que en ellas se contenia
		void borrarSeps(int cantidad,int FilaOColumna);
		void borrarTodo();
		void alargarFilas(int Y);
		void alargarColumnas(int X);
		void actualizarMatriz(int cantFilas,int cantColumnas);
		void cambiarButons();
		void cambiarButonsColumnas();
		/** Pone un punto en la celda que se selecciona.
		 * @param x: Posicion en x de la celda
		 * @param y: Posicion en y de la celda
		 */
		void ponerPunto(int x , int y);

		//Handlers propios
		
		/** Marca a una celda para volcarle informacion.
		 */
		virtual bool on_click_tablero(GdkEventButton* event);
		void on_click_boton_tablero(int id);


		//Senal a emitir para el checkbutton cuando hago un click
		type_signal_uncheck m_signal_uncheck;
};

#endif

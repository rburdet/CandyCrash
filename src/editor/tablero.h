#ifndef TABLERO_H
#define TABLERO_H


#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <glibmm/listhandle.h>
#include <gtkmm.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "celda.h"
#include "columna.h"
//#include "probsetter.h"

class Tablero{
	public:
		Tablero(Glib::RefPtr<Gtk::Builder>& builder);
		~Tablero();
		//Handlers accedidos desde la configuracion basica
		void on_adjCols_changed_tablero(Gtk::SpinButton* spinbutton,int id);
		void on_cordx_changed(Gtk::SpinButton* spin_x);
		void on_cordy_changed(Gtk::SpinButton* spin_y);
		void on_adj_changed_tablero(Gtk::SpinButton* spinbutton,int id);
		void on_image_changed_tablero(Gtk::FileChooser* fileChooser);
		void on_image_fondo_changed_tablero(Gtk::FileChooser* fileChooser);
		void on_check_button_tablero();

		//Metodos serializadores
		void jsonCeldas(Json::Value& nivel,const std::string& nombre);
		void jsonColumnas(Json::Value& nivel,const std::string& nombre);
		
		//Senal del que conectara el checkbutton
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

		//PRUEBA:
		std::vector<std::vector<std::vector< int > > >matrizButons;
		void crearMatrizBotones(int x , int y);
		//Columnas para la probabilidad seteada
		std::vector< Columna* > columnas;
		
		//Tablero con su respectivo event box (fue necesario para tomar los clicks hechos sobre el mismo)
		Gtk::Fixed* tablero;
		Gtk::EventBox* eventos_tablero;

		Gtk::Image* point;
		//Spinbuttons que cambiaron cada vez que se eligio una celda
		std::vector< Gtk::SpinButton* > butonsCambiados;
		std::vector< Gtk::SpinButton* > butonsColumnasCambiados;

		//Datos del tablero
		int cantColumnas;
		int cantFilas;

		//Para tener una referencia de lo ultimo que se toco y no volver a pedir informacion:
		int ultFilClick;
		int ultColClick;
		

		// ***************************************************************//
		//								METODOS							  //
		//Metodos propios 
		void agregarFilas(int X);
		void agregarColumnas(int Y);
		void borrarSeps();
		void alargarFilas(int Y);
		void alargarColumnas(int X);
		void actualizarMatriz(int cantFilas,int cantColumnas);
		void cambiarButons();
		void cambiarButonsColumnas();
		void ponerPunto(int x , int y);

		//Handlers propios
		virtual bool on_click_tablero(GdkEventButton* event);
		void on_click_boton_tablero(int id);


		//Senal a emitir para el checkbutton cuando hago un click
		type_signal_uncheck m_signal_uncheck;


};

#endif

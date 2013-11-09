#ifndef TABLERO_H
#define TABLERO_H

#include <glibmm/listhandle.h>
#include <gtkmm.h>
#include <iostream>
#include <vector>
#include "celda.h"
#include "columna.h"
//#include "probsetter.h"

class Tablero{
	public:
		Tablero(Glib::RefPtr<Gtk::Builder>& builder);
		~Tablero();
		//Handlers accedidos desde la configuracion basica
		void on_cordx_changed(Gtk::SpinButton* spin_x);
		void on_cordy_changed(Gtk::SpinButton* spin_y);
		//TODO: HACERLO PRIVADO Y PONERLE GETTER
		Celda* celdaInteres;
		Columna* colInteres;
		void on_adj_changed_tablero(Gtk::SpinButton* spinbutton,int id);
		void on_adjCols_changed_tablero(Gtk::SpinButton* spinbutton,int id);
		void getProbCeldas();

		std::vector<std::vector<Celda*> > matrizCeldas;
		std::vector< Columna* > columnas;
	private:
		Gtk::Fixed* tablero;
		Gtk::EventBox* eventos_tablero;
		std::vector< Gtk::SpinButton* > butonsCambiados;
		virtual bool on_click_tablero(GdkEventButton* event);
		void on_click_boton_tablero(int id);
		int alto;
		int ancho;
		int cantColumnas;
		int cantFilas;
		//Para tener una referencia de lo ultimo que se toco y no volver a pedir informacion:
		int ultFilClick;
		int ultColClick;

		void agregarFilas(int X);
		void agregarColumnas(int Y);
		void borrarSeps();
		void alargarFilas(int Y);
		void alargarColumnas(int X);
		
		void actualizarMatriz(int cantFilas,int cantColumnas);
		void cambiarButons();

};

#endif

#ifndef TABLERO_H
#define TABLERO_H

#include <gtkmm.h>
#include <iostream>
//#include <algorithm>
#include <vector>

class Tablero{
	public:
		Tablero(Glib::RefPtr<Gtk::Builder>& builder);
		~Tablero();
		//Handlers accedidos desde la configuracion basica
		void on_cordx_changed(Gtk::SpinButton* spin_x);
		void on_cordy_changed(Gtk::SpinButton* spin_y);

	private:
		Gtk::Fixed* tablero;
		int alto;
		int ancho;
		int cantColumnas;
		int cantFilas;

		void agregarFilas(int X);
		void agregarColumnas(int Y);
		
		//TODO: NO ESTA FUNCIONANDO
		void borrarFilas(int X);
		void borrarColumnas(int Y);
};

#endif

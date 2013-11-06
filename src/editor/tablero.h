#ifndef TABLERO_H
#define TABLERO_H

#include <glibmm/listhandle.h>
#include <gtkmm.h>
#include <iostream>
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
		Gtk::EventBox* eventos_tablero;

		virtual bool on_click_tablero(GdkEventButton* event);

		int alto;
		int ancho;
		int cantColumnas;
		int cantFilas;
		void agregarFilas(int X);
		void agregarColumnas(int Y);
		
		void borrarSeps();
};

#endif

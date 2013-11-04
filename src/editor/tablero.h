#ifndef TABLERO_H
#define TABLERO_H

#include <gtkmm.h>
#include <iostream>

class Tablero{
	public:
		Tablero(Glib::RefPtr<Gtk::Builder>& builder);
		~Tablero();
		Gtk::Fixed* getTablero();
		void on_cordx_changed();

	private:
		//Handlers
		Gtk::Fixed* tablero;
		void on_cordy_changed();
};

#endif

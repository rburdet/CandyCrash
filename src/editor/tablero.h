#ifndef TABLERO_H
#define TABLERO_H

#include <gtkmm.h>
#include <iostream>

class Tablero{
	public:
		Tablero(Glib::RefPtr<Gtk::Builder>& builder);
		~Tablero();
		void on_cordx_changed(Gtk::SpinButton* spin_x);
		void on_cordy_changed(Gtk::SpinButton* spin_y);

	private:
		//Handlers
		Gtk::Fixed* tablero;
};

#endif

#ifndef COLUMNA_H
#define COLUMNA_H

#include <vector>
#include <gtkmm.h>

#define NUMEROPIEZAS 16

class Columna{
	public:
		Columna(int x, Gtk::Button* );
		~Columna();
		void on_adj_changed(Gtk::SpinButton* spinbutton,int id);
		Gtk::Button* get_boton();
	private:
		Gtk::Button* boton;
		int pos;
		int prob_piezas[NUMEROPIEZAS];
};

#endif 

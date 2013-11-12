#ifndef COLUMNA_H
#define COLUMNA_H

#include <vector>
#include <gtkmm.h>
#include "info.h"


class Columna{
	public:
		Columna(int x, Gtk::Button* );
		~Columna();
		void on_adj_changed(Gtk::SpinButton* spinbutton,int id);
		Gtk::Button* get_boton();
		Info* getInfo();
	private:
		Gtk::Button* boton;
		int pos;
		Info* probs;
};

#endif 

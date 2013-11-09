#ifndef CELDA_H
#define CELDA_H

#include <string>
#include <vector>
#include <gtkmm.h>

#define NUMEROPIEZAS 16

class Celda{
	public:
		Celda(int x, int y);
		int getX();
		int getY();
		void on_adj_changed(Gtk::SpinButton* spinbutton,int id);
		int prob_piezas[NUMEROPIEZAS];

	private:
		int x;
		int y;
		std::string img;
		void inicializarVector();
};

#endif

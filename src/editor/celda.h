#ifndef CELDA_H
#define CELDA_H

#include <string>
#include <gtkmm.h>

class Celda{
	public:
		Celda(int x, int y);
		int getX();
		int getY();
		void on_adj21_changed(Gtk::SpinButton* spinbutton21);
	private:
		int x;
		int y;
		//Info* info;
		std::string img;
};

#endif

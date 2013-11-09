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

	private:
		int x;
		int y;
		int prob_piezas[NUMEROPIEZAS];
		//int prob_red_button , prob_blue_button , prob_green_button , prob_purple_button , prob_yellow_button;
		//int prob_red_barhor , prob_blue_barhor , prob_green_barhor , prob_purple_barhor , prob_yellow_barhor;
		//int prob_red_barver , prob_blue_barver , prob_green_barver , prob_purple_barver , prob_yellow_barver;
		//int prob_star;
		std::string img;
		void inicializarVector();
};

#endif

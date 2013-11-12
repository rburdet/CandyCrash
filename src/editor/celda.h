#ifndef CELDA_H
#define CELDA_H

#include <string>
#include <vector>
#include <gtkmm.h>
#include "info.h"

#define NUMEROPIEZAS 16

class Celda{
	public:
		Celda(int x, int y);
		~Celda();
		int getX();
		int getY();

		Info* getInfo();
		void on_adj_changed(Gtk::SpinButton* spinbutton,int id);
		void setImage(const std::string& fileName);
		void setHueco();
		bool isHueco();
		std::string getImage();
	private:
		int x;
		int y;
		Info* probs;
		bool hueco;
		std::string imgPath;
		int prob_piezas[NUMEROPIEZAS];
};

#endif

#ifndef GENERAL_PROBS_H
#define GENERAL_PROBS_H

#include <gtkmm.h>
#include <string>
#include <sstream>
#include <vector>
#include "tablero.h"

class Probs{
	public:
		Probs(Tablero* tablero, Glib::RefPtr<Gtk::Builder>& builder, int startNumber, int finalNumber,std::string frameName);

	protected: 
		void cargarBotones(Glib::RefPtr<Gtk::Builder>& builder, int firstbutton,int lastbutton);
		Tablero* tablero;
		std::vector<Gtk::SpinButton *> spinbuttons;
		std::vector< Glib::RefPtr< Gtk::Adjustment > > adjustments;
	private:
		Gtk::Frame * aFrame;
		std::vector< Gtk::Image * >candys;


};

#endif

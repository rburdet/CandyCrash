#ifndef PROB_CELDAS_H
#define PROB_CELDAS_H

#include <gtkmm.h>
#include "generalprobs.h"
#include "tablero.h"
#include <vector>
#include <sstream>

class ProbCeldas : public Probs{
	public:
		ProbCeldas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero);

	private:

		Gtk::Frame * frameProbCeldas;
		std::vector<Gtk::SpinButton *> spinbuttons;
		std::vector< Glib::RefPtr< Gtk::Adjustment > > adjustments;
};

#endif


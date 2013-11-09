#ifndef PROB_CELDAS_H
#define PROB_CELDAS_H

#include <gtkmm.h>
#include "generalprobs.h"
#include <vector>
#include <sstream>

class ProbCeldas : public Probs{
	public:

		ProbCeldas(Tablero* tablero,Glib::RefPtr<Gtk::Builder>& builder);
};

#endif


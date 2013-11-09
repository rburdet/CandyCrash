#ifndef PROB_COLUMNAS_H
#define PROB_COLUMNAS_H

#include <gtkmm.h>
#include "generalprobs.h"
#include "tablero.h"

class ProbColumnas : public Probs{
	public:
		ProbColumnas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero);
	private:
		Gtk::Frame * frameProbColumnas;
};

#endif

#ifndef PROB_COLUMNAS_H
#define PROB_COLUMNAS_H

#include <gtkmm.h>
#include "generalprobs.h"

class ProbColumnas : public Probs{
	public:
		ProbColumnas(Tablero* tablero,Glib::RefPtr<Gtk::Builder>& builder);

	private:
		Gtk::FileChooser* fileChooser;

};

#endif

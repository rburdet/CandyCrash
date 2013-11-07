#ifndef PROB_COLUMNAS_H
#define PROB_COLUMNAS_H

#include <gtkmm.h>
#include "generalprobs.h"

class ProbColumnas : public Probs{
	public:
		ProbColumnas(Glib::RefPtr<Gtk::Builder>& builder);
	private:
		Gtk::Frame * frameProbColumnas;
		Gtk::Image * redbutton;

};

#endif

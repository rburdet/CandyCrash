#ifndef PROB_CELDAS_H
#define PROB_CELAS_H

#include <gtkmm.h>
#include "generalprobs.h"

class ProbCeldas : public Probs{
	public:
		ProbCeldas(Glib::RefPtr<Gtk::Builder>& builder);
};

#endif

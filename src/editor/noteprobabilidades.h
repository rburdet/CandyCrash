#ifndef NOTE_PROBABILIDADES_H
#define NOTE_PROBABILIDADES_H

#include <gtkmm.h>
#include "probcolumnas.h"
//#include "probceldas.h"

class NoteProbabilidades{
	public:
		NoteProbabilidades(Glib::RefPtr<Gtk::Builder>& builder);

	private:
		Gtk::Notebook* noteprob;
		ProbColumnas * probcolumnas;
		//ProbCeldas * probceldas;
};

#endif

#ifndef NOTE_PROBABILIDADES_H
#define NOTE_PROBABILIDADES_H

#include <gtkmm.h>
#include "probcolumnas.h"
#include "probceldas.h"

/** Clase contenedora de las probabilidades de celda y columna dentro del
 * notebook de probabilidades.
 */
class NoteProbabilidades{
	public:
		NoteProbabilidades(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero);

	private:
		Gtk::Notebook* noteprob;
		ProbColumnas * probcolumnas;
		ProbCeldas * probceldas;
};

#endif

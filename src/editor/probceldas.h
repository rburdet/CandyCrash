#ifndef PROB_CELDAS_H
#define PROB_CELDAS_H

#include <gtkmm.h>
#include "generalprobs.h"
#include <vector>
#include <sstream>


/** Clase contenedora de las opciones de la celda
 */
class ProbCeldas : public Probs{
	public:
		/** Se crea el frame con un filechooser, un boton para el hueco 
		 * y se conectan los spinbuttons.
		 */
		ProbCeldas(Tablero* tablero,Glib::RefPtr<Gtk::Builder>& builder);
		~ProbCeldas();

	private:
		Gtk::FileChooser* fileChooser;
		Gtk::Button* huecoButton;
};

#endif


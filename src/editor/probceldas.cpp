#include "probceldas.h"

#define FIRSTIMAGE 17
#define LASTIMAGE 32
#define FIRSTBUTTON 21
#define LASTBUTTON 36

ProbCeldas::ProbCeldas(Tablero* tablero, Glib::RefPtr<Gtk::Builder>& builder) : Probs(tablero ,builder,FIRSTIMAGE,LASTIMAGE,"frameceldas"){
	cargarBotones(builder,FIRSTBUTTON,LASTBUTTON);
}

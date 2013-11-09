#include "probcolumnas.h"

#define FIRSTIMAGE 1 
#define LASTIMAGE 16

ProbColumnas::ProbColumnas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero) : Probs(builder,FIRSTIMAGE,LASTIMAGE,"framecolumnas"){
}

#include "probcolumnas.h"

#define FIRSTIMAGE 1 
#define LASTIMAGE 16

ProbColumnas::ProbColumnas(Tablero* tablero ,Glib::RefPtr<Gtk::Builder>& builder) : Probs(tablero, builder,FIRSTIMAGE,LASTIMAGE,"framecolumnas"){
}

#include "probcolumnas.h"

#define FIRSTIMAGE 1 
#define LASTIMAGE 16
#define FIRSTBUTTON 1
#define LASTBUTTON 16 

ProbColumnas::ProbColumnas(Tablero* tablero ,Glib::RefPtr<Gtk::Builder>& builder) : Probs(tablero, builder,FIRSTIMAGE,LASTIMAGE,"framecolumnas"){
	cargarBotones(builder,FIRSTBUTTON,LASTBUTTON);
	for ( int i = FIRSTBUTTON ; i <= LASTBUTTON ; i++ ){
		adjustments[i-FIRSTBUTTON]->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero,&Tablero::on_adjCols_changed_tablero),spinbuttons[i-FIRSTBUTTON],i-FIRSTBUTTON));
	}
}

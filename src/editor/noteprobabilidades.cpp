#include "noteprobabilidades.h"

NoteProbabilidades::NoteProbabilidades(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero){
	builder->get_widget("noteprobs",this->noteprob);
	//this->probcolumnas = new ProbColumnas(builder);
	//this->probceldas = new ProbCeldas(builder,tablero);
}

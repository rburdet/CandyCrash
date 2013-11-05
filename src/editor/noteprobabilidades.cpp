#include "noteprobabilidades.h"

NoteProbabilidades::NoteProbabilidades(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("noteprobs",this->noteprob);
	this->probcolumnas = new ProbColumnas(builder);
}

#include "probcolumnas.h"

ProbColumnas::ProbColumnas(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("framecolumnas",frameProbColumnas);
}

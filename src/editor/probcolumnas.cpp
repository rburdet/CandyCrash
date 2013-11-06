#include "probcolumnas.h"

ProbColumnas::ProbColumnas(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("framecolumnas",frameProbColumnas);
	builder->get_widget("image1",redbutton);
	redbutton->set("../../imagenes/button_red.png");
	redbutton->show();
}

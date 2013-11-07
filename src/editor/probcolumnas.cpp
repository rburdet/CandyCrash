#include "probcolumnas.h"

#define FIRSTIMAGE 1 
#define LASTIMAGE 16

ProbColumnas::ProbColumnas(Glib::RefPtr<Gtk::Builder>& builder) : Probs(builder,FIRSTIMAGE,LASTIMAGE,"framecolumnas"){
	//builder->get_widget("framecolumnas",frameProbColumnas);
	//builder->get_widget("image1",redbutton);
	//redbutton->set("../../imagenes/button_red.png");
	//redbutton->show();
}

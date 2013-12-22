#include "probceldas.h"

#define FIRSTIMAGE 17
#define LASTIMAGE 32
#define FIRSTBUTTON 21
#define LASTBUTTON 36
#define DEFAULTDIR "../share/candycrash/imagenes/"

ProbCeldas::ProbCeldas(Tablero* tablero, Glib::RefPtr<Gtk::Builder>& builder)
	: Probs(tablero ,builder,FIRSTIMAGE,LASTIMAGE,"framecelda"){
	builder->get_widget("filechooser",fileChooser);
	builder->get_widget("huecobutton",huecoButton);
	fileChooser->set_current_folder(DEFAULTDIR);
	fileChooser->signal_selection_changed().connect(sigc::bind(sigc::mem_fun
				(tablero,&Tablero::on_image_changed_tablero),fileChooser));
	huecoButton->signal_clicked().connect(sigc::mem_fun
			(tablero,&Tablero::on_check_button_tablero));
	cargarBotones(builder,FIRSTBUTTON,LASTBUTTON);
	for ( int i = FIRSTBUTTON ; i <= LASTBUTTON ; i++ ){
		adjustments[i-FIRSTBUTTON]->signal_value_changed().connect(sigc::bind
				(sigc::mem_fun(tablero,&Tablero::on_adj_changed_tablero),
				 spinbuttons[i-FIRSTBUTTON],i-FIRSTBUTTON));
	}
}

ProbCeldas::~ProbCeldas(){
}

#include "probceldas.h"

#define FIRSTIMAGE 17
#define LASTIMAGE 32

ProbCeldas::ProbCeldas(Glib::RefPtr<Gtk::Builder>& builder, Tablero* tablero) : Probs(builder,FIRSTIMAGE,LASTIMAGE,"frameceldas"){
	
	for ( int i = 21 ; i <= 36 ; i++ ){
		std::stringstream ss;
		Gtk::SpinButton* aux;
		Glib::RefPtr< Gtk::Adjustment > aux2;
		ss << "spinbutton" << i; 
		builder->get_widget(ss.str(),aux);
		spinbuttons.push_back(aux);
		std::stringstream ss2;
		ss2 << "adjustment" << i;
		aux2= Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object(ss2.str()));
		aux2->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero,&Tablero::on_adj_changed_tablero),spinbuttons[i-21],i-21));
		adjustments.push_back(aux2);
	}

	std::cout << " celdaInteres " << tablero->celdaInteres << "\n";
}

#include "barraestado.h"
#include <string>
#define DEFAULTDIR "../../Mapas/"

BarraEstado::BarraEstado(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("barraestado",barraEstado);
}

void BarraEstado::onMostrar(Gtk::Entry* entryArchivo){
	std::string dir = std::string(DEFAULTDIR);
	std::string textoAMostrar = "Se ha generado "+ dir + entryArchivo->get_text();
	barraEstado->push(textoAMostrar);
}

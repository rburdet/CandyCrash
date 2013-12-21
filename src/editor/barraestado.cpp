#include "barraestado.h"
#include <string>
#define DEFAULTDIR "../../Mapas/"

BarraEstado::BarraEstado(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("barraestado",barraEstado);
}


void BarraEstado::mostrar(const std::string& texto){
	barraEstado->push(texto);
}

void BarraEstado::onMostrar(Gtk::Entry* entryArchivo){
	std::string dir = std::string(DEFAULTDIR);
	std::string textoAMostrar = "Se ha generado "+ dir + entryArchivo->get_text();
	mostrar(textoAMostrar);
}

BarraEstado::~BarraEstado(){
	delete barraEstado;
}

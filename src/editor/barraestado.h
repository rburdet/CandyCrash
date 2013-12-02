#ifndef BARRAESTADO_H
#define BARRAESTADO_H

#include <gtkmm.h>
#include <string>

class BarraEstado{
	public:
		explicit BarraEstado(Glib::RefPtr<Gtk::Builder>& builder);
		void onMostrar(Gtk::Entry* entryArchivo);
	private:
		Gtk::Statusbar* barraEstado;
};

#endif

#ifndef BOX_OPCIONES_H
#define BOX_OPCIONES_H

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <iostream>
#include <gtkmm.h>
#include <string>

class BoxOpcionesBasicas {
	public:
		BoxOpcionesBasicas(Glib::RefPtr<Gtk::Builder>& builder);
		~BoxOpcionesBasicas();
	private:
		Gtk::Button * button_json;
		Gtk::Entry * e_nombre;
		Gtk::SpinButton * s_maxjug;
		Gtk::SpinButton * s_puntaje;
		Gtk::SpinButton * spin_x;
		Gtk::SpinButton * spin_y;
		Glib::RefPtr<Gtk::Adjustment> adjustment_cordx;

		//Handlers
		void on_button_clicked();
		void on_cordx_changed();

		//Metodos
		std::string getNombre();
		double getPuntaje();
		double getJugadoresMax();
		double getDimX();
		double getDimY();
};

#endif

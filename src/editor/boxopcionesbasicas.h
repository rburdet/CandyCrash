#ifndef BOX_OPCIONES_H
#define BOX_OPCIONES_H

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <iostream>
#include <gtkmm.h>
#include <string>
#include <sstream>
#include "tablero.h"
#include "persistidor.h"
#include "barraestado.h"

class BoxOpcionesBasicas{
	public:
		BoxOpcionesBasicas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero);
		~BoxOpcionesBasicas();

		//TODO: encapsular bien
		Glib::RefPtr<Gtk::Adjustment> adjustment_cordx;
		Glib::RefPtr<Gtk::Adjustment> adjustment_cordy;
		Glib::RefPtr<Gtk::Adjustment> adjustment_nivel;


	private:
		Gtk::Button * button_json;
		Gtk::Entry * e_nombre;
		Gtk::SpinButton * s_maxjug;
		Gtk::SpinButton * s_puntaje;
		Gtk::SpinButton * spin_x;
		Gtk::SpinButton * spin_y;
		Gtk::SpinButton * spin_nivel;
		Tablero* tablero;
		BarraEstado * barraEstado;

		//Handlers
		void on_button_clicked();

		//Metodos
		std::string getNombre();
		double getPuntaje();
		double getJugadoresMax();
		int getDimX();
		int getDimY();
		int getNivel();
};

#endif

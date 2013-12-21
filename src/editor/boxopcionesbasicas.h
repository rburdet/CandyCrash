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

/** Panel izquierdo 
 */
class BoxOpcionesBasicas{
	public:
		/** Constructor del panel izquierdo superior usado para la configuracion
		 * basica del nivel
		 * @param builder[in]: Encargado de levantar el widget desde el archivo
		 * de glade.
		 */
		BoxOpcionesBasicas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero);
		~BoxOpcionesBasicas();

		Glib::RefPtr<Gtk::Adjustment> adjustment_cordx;
		Glib::RefPtr<Gtk::Adjustment> adjustment_cordy;
		Glib::RefPtr<Gtk::Adjustment> adjustment_nivel;
		void generar();
		BarraEstado* getBarra();
		std::string getNombre();
		void cargar(const std::string& fileName);


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
		/** Handler para cuando se presiona el boton generar.
		 * Serializa el mapa con toda la informacion puesta
		 */
		void on_button_clicked();

		//Metodos
		double getPuntaje();
		double getJugadoresMax();
		int getDimX();
		int getDimY();
		int getNivel();
};

#endif

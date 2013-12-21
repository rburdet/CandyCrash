#ifndef MENU_PERSISTIR_H
#define MENU_PERSISTIR_H

#include <gtkmm.h>
#include <string>
#include "boxopcionesbasicas.h"
#include "barraestado.h"

/** Clase para encapsular metodos de persistencia del editor
 */

class MenuPersistidor{
	public:
		MenuPersistidor(BoxOpcionesBasicas* aPersistir, Glib::RefPtr<Gtk::Builder>& builder);
		~MenuPersistidor();

	private:
		//Gtk::MenuBar* menu;
		Gtk::ImageMenuItem* itemNuevo;
		Gtk::ImageMenuItem* itemAbrir;
		Gtk::ImageMenuItem* itemGuardar;
		//Gtk::ImageMenuItem* itemGuardar_Como;
		BoxOpcionesBasicas* settings;
		void Nuevo();
		void Abrir();
		void Guardar();
		//void Guardar_Como();
};

#endif


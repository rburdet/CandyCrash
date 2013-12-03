#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "boxopcionesbasicas.h"
#include "tablero.h"
#include "menu.h"

/** Ventana del editor
 */
class MainWindow : public Gtk::Window{
	public:
		MainWindow();
		virtual ~MainWindow();
		Gtk::Window* getMainWindow();

	protected:
		Gtk::Window * mainWindow;

	private:
		/** Cierra la ventana
		 */
		void cerrar();

		Glib::RefPtr<Gtk::Builder> builder;
		BoxOpcionesBasicas* boxOpcionesBasicas;
		Tablero* tablero;
		Menu* menu;
		Gtk::ImageMenuItem* close;
};

#endif 

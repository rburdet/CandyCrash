#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "boxopcionesbasicas.h"
#include "tablero.h"
#include "menu.h"

class MainWindow : public Gtk::Window{
	public:
		MainWindow();
		virtual ~MainWindow();
		Gtk::Window* getMainWindow();

	protected:
		Gtk::Window * mainWindow;

	private:
		Glib::RefPtr<Gtk::Builder> builder;
		BoxOpcionesBasicas* boxOpcionesBasicas;
		Tablero* tablero;
		Menu* menu;
};

#endif 

#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>
#include "boxopcionesbasicas.h"
#include "tablero.h"
#include "noteprobabilidades.h"

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
		NoteProbabilidades* noteProb;

		Tablero* tablero;
		//Gtk::Button * button_json;
		//Gtk::Entry * e_nombre;
		//Gtk::SpinButton * s_maxjug;
		//Gtk::SpinButton * s_puntaje;
		// Signal handlers:
		//void on_button_clicked();
		//std::string getNombre();
		//double getPuntaje();
		//double getJugadoresMax();
		//std::string getDimX();
		//std::string getDimY();
		// Child widgets:
		//Gtk::Table m_table,m_tableDim;
		//Gtk::Button m_button;
		//Gtk::Label label_nombre, label_puntaje,label_maxjug,label_dimx,label_dimy;
		//Gtk::CheckButton m_check;
		//Gtk::Entry m_entry_nombre, m_entry_puntaje,m_entry_maxjug,m_entry_dimx,m_entry_dimy;
};

#endif /* GTKMM_EXAMPLEWINDOW_H */

#ifndef MENU_H
#define MENU_H

#include <gtkmm.h>
#include <string>

class Menu{
	public:
		Menu(Glib::RefPtr<Gtk::Builder>& builder);

	private:

		Gtk::Window* window;
		Gtk::MenuBar* menubar;
		Gtk::ImageMenuItem* close;
		Gtk::ImageMenuItem* about;
		void ayuda();
		void cerrar();

};
#endif

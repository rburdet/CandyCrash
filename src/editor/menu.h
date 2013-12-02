#ifndef MENU_H
#define MENU_H

#include <gtkmm.h>
#include <string>

class Menu{
	public:
		explicit Menu(Glib::RefPtr<Gtk::Builder>& builder);

	private:
		Gtk::Window* window;
		Gtk::MenuBar* menubar;
		Gtk::ImageMenuItem* about;
		Gtk::ImageMenuItem* help;
		void displayHelp();
		void displayAbout();
};

#endif

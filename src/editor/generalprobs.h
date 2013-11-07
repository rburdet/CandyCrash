#ifndef GENERAL_PROBS_H
#define GENERAL_PROBS_H

#include <gtkmm.h>
#include <string>
#include <sstream>
#include <vector>

class Probs{
	public:
		Probs(Glib::RefPtr<Gtk::Builder>& builder, int startNumber, int finalNumber,std::string frameName);

	private:
		Gtk::Frame * aFrame;

		//Botones
		Gtk::Image * red_button;
		Gtk::Image * blue_button;
		Gtk::Image * green_button;
		Gtk::Image * purple_button;
		Gtk::Image * yellow_button;

		//Minibars
		//Horizontales
		Gtk::Image * red_minibar_hor;
		Gtk::Image * blue_minibar_hor;
		Gtk::Image * green_minibar_hor;
		Gtk::Image * purple_minibar_hor;
		Gtk::Image * yellow_minibar_hor;

		//Verticales
		Gtk::Image * red_minibar_ver;
		Gtk::Image * blue_minibar_ver;
		Gtk::Image * green_minibar_ver;
		Gtk::Image * purple_minibar_ver;
		Gtk::Image * yellow_minibar_ver;

		//Estrella
		Gtk::Image * star;

};

#endif

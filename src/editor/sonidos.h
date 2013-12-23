#ifndef SONIDOS_H
#define SONIDOS_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include "../cliente/cliente.sound_player.h"

class Sonidos{
	public:
		Sonidos(Glib::RefPtr<Gtk::Builder>& builder);
		~Sonidos();
		void jsonSonidos(Json::Value& nivel,const std::string& nombre);
		void reproducir(Gtk::FileChooser* fileChooser);
		//void guardar(Gtk::FileChooser* fileChooser,std::string sound);

	private:
		Gtk::Button* buttonrep1;
		Gtk::Button* buttonrep2;
		Gtk::Button* buttonrep3;
		Gtk::Button* buttonrep4;
		Gtk::FileChooser* fileChooser1;
		Gtk::FileChooser* fileChooser2;
		Gtk::FileChooser* fileChooser3;
		Gtk::FileChooser* fileChooser4;
};
#endif

#ifndef __CLIENTE_GAME_H__
#define __CLIENTE_GAME_H__
#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "cliente.window.h"

class GameWindow : public Window {
	public:
		GameWindow();
		virtual ~GameWindow();
		virtual void mensaje(Json::Value& data);

	protected:
		void on_mensaje();

		Gtk::VBox m_VBox;
		Gtk::ScrolledWindow m_ScrolledWindow1;
		Gtk::ScrolledWindow m_ScrolledWindow2;
		Gtk::TextView m_TextView1;
		Gtk::TextView m_TextView2;
		Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1, m_refTextBuffer2;
		Gtk::Button m_button_send;
};

#endif

#ifndef __CLIENTE_WINDOW_H__
#define __CLIENTE_WINDOW_H__
#include <jsoncpp/json/json.h>
#include <gtkmm.h>

class Window : public Gtk::Window {
	public:
		virtual void mensaje(Json::Value& data) = 0;

		typedef sigc::signal<void, Json::Value> type_signal_mensaje;
		type_signal_mensaje signal_mensaje(){
			return m_signal_mensaje;
		}

	protected:
		type_signal_mensaje m_signal_mensaje;

};

#endif

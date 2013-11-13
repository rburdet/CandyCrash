#ifndef __CLIENTE_WINDOW_H__
#define __CLIENTE_WINDOW_H__
#include <jsoncpp/json/json.h>
#include <gtkmm.h>

class Window : public Gtk::Window {
	public:
		virtual void mensaje(Json::Value& data) = 0;

};

#endif

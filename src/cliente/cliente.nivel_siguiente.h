#ifndef NIVEL_SIG_WINDOW_H
#define NIVEL_SIG_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>
#include "cliente.window.h"
#include "../common/common.events.h"

#include "widgets/cliente.mapas_list.h"

/** Ventana de login y seleccionar servidor.
 */
class NivSigWindow : public Window {
	public:
		NivSigWindow();
		virtual ~NivSigWindow();

		virtual void mensaje(Json::Value& data);

		typedef sigc::signal<void> type_signal_salir;
		typedef sigc::signal<void, std::string, int> type_signal_crear;
		type_signal_salir signal_salir();
		type_signal_crear signal_crear();

	protected:
		//Child widgets:
		Gtk::Box m_HBox;
		Gtk::VBox m_VBox;

		Gtk::Label m_text;
		ListaMapas partidas;
		Gtk::ScrolledWindow m_ScrolledMapas;
		Gtk::HBox but_box;
		Gtk::Button m_Button_crear;
		Gtk::Button m_Button_salir;

	private:
		void on_button_crear();
		void on_button_salir();

		type_signal_salir m_signal_salir;
		type_signal_crear m_signal_crear;
};

#endif

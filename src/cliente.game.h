#ifndef __CLIENTE_GAME_H__
#define __CLIENTE_GAME_H__
#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "cliente.window.h"
#include "cliente.tablerojuego.h"
#include "widgets/cliente.usuarios_list.h"

/** Ventana de espera antes de que se mande la partida a jugar.
 * Basicamente es un chat
 */
class GameWindow : public Window {
	public:
		GameWindow();
		virtual ~GameWindow();
		virtual void mensaje(Json::Value& data);

	protected:
		/** Signal handler del click en el boton de send
		 */
		void on_mensaje();
		void on_start_game();
		void on_salir_game();
		void on_tablero_mensaje(Json::Value data);

		Gtk::VBox m_VBox;
		Gtk::ScrolledWindow m_ScrolledWindow1;
		Gtk::TextView m_TextView1;
		Gtk::HBox m_HBox;
		Gtk::Entry text_input;
		Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
		Gtk::Button m_button_send;
		ListaUsuarios user_list;
		Gtk::HBox but_hbox;
		Gtk::Button button_start;
		Gtk::Button button_salir;
		TableroJuego* tableroJuego;
};

#endif

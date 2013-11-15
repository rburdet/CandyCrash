#ifndef __CLIENTE_GAME_H__
#define __CLIENTE_GAME_H__
#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "cliente.window.h"
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

		Gtk::VBox m_VBox;
		Gtk::ScrolledWindow m_ScrolledWindow1;
		Gtk::ScrolledWindow m_ScrolledWindow2;
		Gtk::TextView m_TextView1;
		Gtk::HBox m_HBox;
		Gtk::TextView m_TextView2;
		Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1, m_refTextBuffer2;
		Gtk::Button m_button_send;
		ListaUsuarios user_list;
		Gtk::Button button_start;
		Gtk::HBox main_h_box;
		Gtk::VBox lista_box;
};

#endif

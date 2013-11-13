#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "cliente.window.h"
#include "widgets/cliente.partidas_list.h"
#include "widgets/cliente.mapas_list.h"

class MainWindow : public Window {

	public:
		MainWindow();
		virtual ~MainWindow();
		//typedef sigc::signal<void, std::string> type_signal_mensaje;
		//type_signal_mensaje signal_mensaje();
		void setText(std::string& str);
		virtual void mensaje(Json::Value& data);

	protected:
		void onListGames(int code, Json::Value& data);

		//Signal handlers:
		void on_mensaje();
		void on_partidas();
		void on_mapas();
		void on_crear_partida();

		//Child widgets:
		Gtk::Notebook tabs;

		Gtk::Box m_VBox;
		Gtk::Label label1;
		Gtk::ScrolledWindow m_ScrolledWindow1;
		Gtk::ScrolledWindow m_ScrolledWindow2;
		Gtk::TextView m_TextView1;
		Gtk::TextView m_TextView2;
		Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1, m_refTextBuffer2;
		Gtk::Button m_button_send;
		void fill_buffers();

		Gtk::Label labelPartidas;
		Gtk::VBox m_VBox_partidas;
		Gtk::Button button_partidas;
		Gtk::ScrolledWindow m_ScrolledPartidas;
		ListaPartidas m_TreeView;

		Gtk::Label labelMapas;
		Gtk::VBox m_VBox_mapas;
		Gtk::HBox m_HBox_mapas_buttons;
		Gtk::Button button_mapas_act;
		Gtk::Button button_mapas_cre;
		Gtk::ScrolledWindow m_ScrolledMapas;
		ListaMapas m_TreeViewMapas;

};

#endif 

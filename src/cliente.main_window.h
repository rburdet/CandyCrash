#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "cliente.window.h"
#include "widgets/cliente.partidas_list.h"

class MainWindow : public Window {

	public:
		MainWindow();
		virtual ~MainWindow();
		typedef sigc::signal<void, std::string> type_signal_mensaje;
		type_signal_mensaje signal_mensaje();
		void setText(std::string& str);
		virtual void mensaje(Json::Value& data);

		void addRow(unsigned int m_col_nivel, unsigned long m_col_id, short m_col_conectados, int m_col_percentage);


	protected:
		void onListGames(int code, Json::Value& data);

		//Signal handlers:
		void on_mensaje();
		void on_partidas();

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
		type_signal_mensaje m_signal_mensaje;
		void fill_buffers();

		Gtk::Label labelPartidas;
		Gtk::VBox m_VBox_partidas;
		Gtk::Button button_partidas;
		Gtk::ScrolledWindow m_ScrolledPartidas;

		//Gtk::TreeView m_TreeView;
		ListaPartidas m_TreeView;


};

#endif 

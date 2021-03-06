#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>

#include "cliente.window.h"
#include "widgets/cliente.menu_bar.h"
#include "widgets/cliente.partidas_list.h"
#include "widgets/cliente.mapas_list.h"

/** Ventana de Seleccionar y/o crear partida
 */
class MainWindow : public Window {
	public:
		MainWindow();
		virtual ~MainWindow();
		void setText(std::string& str);
		virtual void mensaje(Json::Value& data);
		/** Metodo para actualizar la lista de partidas.
		 */
		void on_partidas(); // -> handler del actualizador de la lista de partidas

		/** Metodo para actualizar la lista de mapas.
		 */
		void on_mapas(); // -> actualizador de lista de mapas

	protected:
		void onListGames(int code, Json::Value& data);
		void onListMaps(int code, Json::Value& data);

		//Signal handlers:
		void join_partidas(); // -> ... unirse a partida
		void on_crear_partida(); // -> crear partida

		//Child widgets:
		MenuBarDisconnect menubar;
		Gtk::Notebook tabs;

		// ------------------

		Gtk::Label labelPartidas;
		Gtk::VBox m_VBox_partidas;
		Gtk::HBox m_HBox_partidas_buttons;
		Gtk::Button button_partidas_act;
		Gtk::Button button_partidas_con;
		Gtk::ScrolledWindow m_ScrolledPartidas;
		ListaPartidas m_TreeView;

		Gtk::Label labelMapas;
		Gtk::VBox m_VBox_mapas;
		Gtk::HBox m_HBox_mapas_buttons;
		Gtk::Button button_mapas_act;
		Gtk::Button button_mapas_cre;
		Gtk::ScrolledWindow m_ScrolledMapas;
		ListaMapas m_TreeViewMapas;

		Gtk::VBox mainV;
		Gtk::HBox tabBox;
		Gtk::Label statusLabel;

		virtual bool onClose();

	private:
		void on_salir();
		void on_desconectar();
		void on_help();
};

#endif 

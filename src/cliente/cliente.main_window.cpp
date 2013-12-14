#include "cliente.main_window.h"
#include "../common/common.events.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;
using Json::StaticString;
using Json::Value;

MainWindow::MainWindow(){
	set_title("Pantalla principal");
	set_size_request(350, 700);

	Glib::RefPtr<Gtk::StyleContext> stylecontext = tabs.get_style_context();
	stylecontext->add_class("tabs");
	stylecontext->context_save();

	mainV.pack_start(menubar, Gtk::PACK_SHRINK, 0);

	menubar.signal_quit().connect(sigc::mem_fun(*this, &MainWindow::on_salir));
	menubar.signal_disconnect().connect(sigc::mem_fun(*this, 
				&MainWindow::on_desconectar));
	menubar.signal_help().connect(sigc::mem_fun(*this,&MainWindow::on_help));
	menubar.signal_about().connect(sigc::mem_fun(*this,&Window::on_about));


	mainV.pack_start(tabBox, true, true, 10);
	tabBox.pack_start(tabs, true, true, 5);
	mainV.pack_start(statusLabel, false, false, 0);

	add(mainV);

	// ----- Partidas ----
	stylecontext = labelPartidas.get_style_context();
	labelPartidas.set_text("Partidas");
	m_ScrolledPartidas.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	button_partidas_act.set_label("Actualizar");
	button_partidas_con.set_label("Conectar");
	m_HBox_partidas_buttons.pack_start(button_partidas_act, true, true, 10);
	m_HBox_partidas_buttons.pack_start(button_partidas_con, true, true, 10);
	m_VBox_partidas.pack_start(m_ScrolledPartidas, true, true, 0);
	m_VBox_partidas.pack_start(m_HBox_partidas_buttons, false, false, 10);

	button_partidas_act.signal_clicked().connect(sigc::mem_fun(*this, 
				&MainWindow::on_partidas));
	button_partidas_con.signal_clicked().connect(sigc::mem_fun(*this, 
				&MainWindow::join_partidas));

	stylecontext = button_partidas_act.get_style_context();
	stylecontext->add_class("btn");
	stylecontext->context_save();

	stylecontext = button_partidas_con.get_style_context();
	stylecontext->add_class("btn1");
	stylecontext->context_save();

	tabs.prepend_page(m_VBox_partidas, labelPartidas);

	stylecontext = m_TreeView.get_style_context();
	stylecontext->add_class("partidas_list");
	stylecontext->context_save();

	m_ScrolledPartidas.add(m_TreeView);

	stylecontext = m_ScrolledPartidas.get_style_context();
	stylecontext->add_class("scrolled-tree");
	stylecontext->context_save();

	// ----- Mapas ----
	labelMapas.set_text("Crear Partida");
	m_ScrolledMapas.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
	button_mapas_act.set_label("Actualizar");
	button_mapas_cre.set_label("Crear Partida");
	m_HBox_mapas_buttons.pack_start(button_mapas_act, true, true, 10);
	m_HBox_mapas_buttons.pack_start(button_mapas_cre, true, true, 10);
	m_VBox_mapas.pack_start(m_ScrolledMapas, true, true, 0);
	m_VBox_mapas.pack_start(m_HBox_mapas_buttons, false, false, 10);

	button_mapas_act.signal_clicked().connect(sigc::mem_fun(*this, 
				&MainWindow::on_mapas));
	button_mapas_cre.signal_clicked().connect(sigc::mem_fun(*this, 
				&MainWindow::on_crear_partida));

	stylecontext = button_mapas_act.get_style_context();
	stylecontext->add_class("btn");
	stylecontext->context_save();

	stylecontext = button_mapas_cre.get_style_context();
	stylecontext->add_class("btn1");
	stylecontext->context_save();

	tabs.prepend_page(m_VBox_mapas, labelMapas);

	stylecontext = m_TreeViewMapas.get_style_context();
	stylecontext->add_class("mapas_list");
	stylecontext->context_save();

	m_ScrolledMapas.add(m_TreeViewMapas);

	stylecontext = m_ScrolledMapas.get_style_context();
	stylecontext->add_class("scrolled-tree");
	stylecontext->context_save();
	// ----------------


	show_all_children();
	show_all();
}

MainWindow::~MainWindow(){
}

void MainWindow::on_partidas(){
	Json::Value msj;
	msj["event"] = EVENT_LIST_GAMES;
	m_signal_mensaje.emit(msj);
}

void MainWindow::join_partidas(){
	Value val;
	unsigned int nivel;
	long id;
	int conectados, percentage;
	std::string nombre;
	this->m_TreeView.getSelected(nivel, id, nombre, conectados, percentage);
	val["event"] = EVENT_JOIN_GAME;
	stringstream ss;
	ss << id;
	val["id"] = ss.str();
	m_signal_mensaje.emit(val);
}

void MainWindow::on_mapas(){
	Json::Value msj;
	msj["event"] = EVENT_GET_MAPS;
	m_signal_mensaje.emit(msj);
}

void MainWindow::on_crear_partida(){
	Value val;
	string name;
	int nivel;
	this->m_TreeViewMapas.getSelected(nivel, name);
	val["event"] = EVENT_NEW_GAME;
	val["nivel"] = nivel;
	val["nombre"] = name;
	m_signal_mensaje.emit(val);
}

void MainWindow::setText(std::string& str){
}

void MainWindow::mensaje(Json::Value& data){
	StaticString def("");
	CommonEvents event = EVENT_NONE;
	int code = -1;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	if(data.get("code", def).isNumeric())
		code = (CommonEvents) data.get("code", def).asInt();

	// para debug
	Json::StyledWriter writer;
	string output = writer.write(data);
	this->setText(output);

	this->statusLabel.set_text(data["msj"].asString());

	switch(event){
		case EVENT_LIST_GAMES:
			this->onListGames(code, data);
			break;
		case EVENT_GET_MAPS:
			this->onListMaps(code, data);
			break;

		default:
			break;
	}
}

void MainWindow::onListGames(int code, Json::Value& data){
	this->m_TreeView.clearRows();
	if(data["partidas"].isArray()){
		Json::Value partidas = data["partidas"];
		int nivel;
		string id;
		unsigned long id_n;
		int users;
		std::string nombre;
		int max_users;
		for(int i =0; i < partidas.size(); i++){
			id = partidas[i]["id"].asString();
			stringstream ss(id);
			ss >> id_n;
			nivel = partidas[i]["nivel"].asInt();
			users = partidas[i]["users"].asInt();
			max_users = partidas[i]["max_users"].asInt();
			nombre = partidas[i]["nombre"].asString();

			this->m_TreeView.addRow(nivel, id_n, nombre, users, 
					max_users ? users*100/max_users: 0);
		}
	}
}

void MainWindow::onListMaps(int code, Json::Value& data){
	this->m_TreeViewMapas.clearRows();
	if(data["mapas"].isObject()){
		Json::Value mapas = data["mapas"];
		Json::Value::Members nombres = mapas.getMemberNames();
		string key;
		int value;
		for(int i =0; i < nombres.size(); i++){
			key = nombres[i];
			value = mapas[key].asInt();
			this->m_TreeViewMapas.addRow(value, key);
		}
	}
}

bool MainWindow::onClose(){
	//TODO: tildes
	if(this->dialog("Desea Cerrar?", "No podra seguir jugando")){
		this->hide();
		Gtk::Main::quit();
	}
	return true;
}

void MainWindow::on_salir(){
	this->onClose();
}

void MainWindow::on_desconectar(){
	if(this->dialog("Esta seguro de desconectarse?", "No podra seguir jugando")){
		Json::Value fake;
		fake["event"] = EVENT_LOGOUT;
		fake["msj"] = "";
		m_signal_mensaje.emit(fake);
	}
}

void MainWindow::on_help(){
	Gtk::MessageDialog dialog(*this, "Ayuda");
	dialog.set_secondary_text("Ud puede: \n"
			" \t Empezar una nueva partida (1) .\n"
			" \t Unirse a una partida ya existente (2) .\n" 
			" \n\n\n " 
			" (1)    En la pestana \"Crear Partida\" presione actualizar. "
			"Esta accion listara todos los mapas disponibles con su respectivo"
			"nivel. Ud podra elegir solo aquellos mapas que sean de un nivel "
			"igual o inferior al suyo. Una vez seleccionado, pulse "
			"\" Crear Partida \" ."
			" \n"
			" (2)    En la pestana \" Partidas \" presione \"Actualizar\". "
			"Esta accion listara todas las partidas creadas, indicando cuan "
			"llena esta la partida, nivel y nombre del mapa a jugar."
			" Seleccione una y presione \"Conectar\" "
			"Recuerde que Ud podra elegir solo aquellas partidas que sean de"
			"un nivel igual o inferior al suyo");
	dialog.run();
}

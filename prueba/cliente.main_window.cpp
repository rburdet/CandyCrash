#include "cliente.main_window.h"
#include "common.events.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;
using Json::StaticString;
using Json::Value;

MainWindow::MainWindow()
	: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_button_send("_Send", true)
{
	set_title("Pantalla principal");
	set_border_width(5);
	set_default_size(400, 200);

	add(tabs);
	//add(m_VBox);

	// ----------
	label1.set_text("debug");

	tabs.prepend_page(m_VBox, label1);

	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
	m_ScrolledWindow1.add(m_TextView1);

	//Only show the scrollbars when they are necessary:
	m_ScrolledWindow1.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_VBox.pack_start(m_ScrolledWindow1);

	m_ScrolledWindow2.add(m_TextView2);
	m_ScrolledWindow2.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_VBox.pack_start(m_ScrolledWindow2);

	//Add buttons:
	m_VBox.pack_start(m_button_send, Gtk::PACK_SHRINK);

	//Connect signals:
	m_button_send.signal_clicked().connect( sigc::mem_fun(*this,
				&MainWindow::on_mensaje) );

	fill_buffers();
	m_TextView1.set_buffer(m_refTextBuffer1);
	m_TextView2.set_buffer(m_refTextBuffer2);

	// ----- Partidas ----
	labelPartidas.set_text("Partidas");
	m_ScrolledPartidas.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	button_partidas_act.set_label("Actualizar");
	button_partidas_con.set_label("Conectar");
	m_HBox_partidas_buttons.pack_start(button_partidas_act);
	m_HBox_partidas_buttons.pack_start(button_partidas_con);
	m_VBox_partidas.pack_start(m_ScrolledPartidas);
	m_VBox_partidas.pack_start(m_HBox_partidas_buttons);

	button_partidas_act.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_partidas));
	button_partidas_con.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::join_partidas));
	tabs.prepend_page(m_VBox_partidas, labelPartidas);

	m_ScrolledPartidas.add(m_TreeView);

	// ----- Mapas ----
	labelMapas.set_text("Crear Partida");
	m_ScrolledMapas.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_VBox_mapas.pack_start(m_ScrolledMapas);
	m_VBox_mapas.pack_start(m_HBox_mapas_buttons);
	button_mapas_act.set_label(string("Actualizar"));
	m_HBox_mapas_buttons.pack_start(button_mapas_act);
	button_mapas_cre.set_label(string("Crear Partida"));
	m_HBox_mapas_buttons.pack_start(button_mapas_cre);

	button_mapas_act.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_mapas));
	button_mapas_cre.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_crear_partida));

	tabs.prepend_page(m_VBox_mapas, labelMapas);

	m_ScrolledMapas.add(m_TreeViewMapas);
	// ----------------


	show_all_children();
	show_all();
}

void MainWindow::fill_buffers(){
	m_refTextBuffer1 = Gtk::TextBuffer::create();
	m_refTextBuffer1->set_text("Buffer 1");

	m_refTextBuffer2 = Gtk::TextBuffer::create();
	m_refTextBuffer2->set_text("Buffer 2");

}

MainWindow::~MainWindow(){
}

void MainWindow::on_mensaje(){
	string str = m_refTextBuffer2->get_text();
	Json::Value data;
	Json::Reader reader;
	if(reader.parse(str, data, false)){
		m_signal_mensaje.emit(data);
	}
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
	std::cout << "Join data: " << nivel << " " << id << " " << ss.str() << std::endl;
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
	m_refTextBuffer1->set_text(str);
}

void MainWindow::mensaje(Json::Value& data){
	StaticString def("");
	CommonEvents event = EVENT_NONE;
	int code = -1;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	if(data.get("code", def).isNumeric())
		code = (CommonEvents) data.get("code", def).asInt();

	Json::StyledWriter writer;
	string output = writer.write(data);
	this->setText(output);

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
		for(unsigned int i =0; i < partidas.size(); i++){
			id = partidas[i]["id"].asString();
			stringstream ss(id);
			ss >> id_n;
			nivel = partidas[i]["nivel"].asInt();
			users = partidas[i]["users"].asInt();
			max_users = partidas[i]["max_users"].asInt();
			nombre = partidas[i]["nombre"].asString();

			this->m_TreeView.addRow(nivel, id_n, nombre, users, max_users ? users*100/max_users: 0);
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
		for(unsigned int i =0; i < nombres.size(); i++){
			key = nombres[i];
			value = mapas[key].asInt();
			this->m_TreeViewMapas.addRow(value, key);
		}
	}

}

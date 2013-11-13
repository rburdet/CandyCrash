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
	m_button_send("_Send", true), button_partidas("Refrescar", true)
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
	// ---------

	labelPartidas.set_text("Partidas");
	m_ScrolledPartidas.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_VBox_partidas.pack_start(m_ScrolledPartidas);
	m_VBox_partidas.pack_start(button_partidas);

	button_partidas.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_partidas));
	tabs.prepend_page(m_VBox_partidas, labelPartidas);

	m_ScrolledPartidas.add(m_TreeView);


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
	string data = m_refTextBuffer2->get_text();
	m_signal_mensaje.emit(data);
}

void MainWindow::on_partidas(){
	//TODO: usar event
	m_signal_mensaje.emit(string("{\"event\":3}"));
}

MainWindow::type_signal_mensaje MainWindow::signal_mensaje(){
	return m_signal_mensaje;
}
void MainWindow::setText(std::string& str){
	m_refTextBuffer1->set_text(str);
}


void MainWindow::addRow(unsigned int m_col_nivel, unsigned long m_col_id, short m_col_conectados, int m_col_percentage){
	this->m_TreeView.addRow(m_col_nivel, m_col_id, m_col_conectados, m_col_percentage);
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
		for(int i =0; i < partidas.size(); i++){
			id = partidas[i]["id"].asString();
			stringstream ss(id);
			ss >> id_n;
			nivel = partidas[i]["nivel"].asInt();
			users = partidas[i]["users"].asInt();
			this->addRow(nivel, id_n, users, 0);
		}
	}

}

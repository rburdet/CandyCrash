#include "cliente.game.h"
#include "common.events.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;
using Json::StaticString;
using Json::Value;

GameWindow::GameWindow() {
	this->add(this->main_h_box);
	this->main_h_box.pack_start(this->lista_box);
	this->lista_box.pack_start(this->user_list);
	this->lista_box.pack_start(this->button_start);
	this->button_start.set_label("Empezar el juego");
	this->button_start.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::on_start_game) );
	this->lista_box.pack_start(this->button_salir);
	this->button_salir.set_label("Salir de la partida");
	this->button_salir.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::on_salir_game) );
	this->main_h_box.pack_start(this->m_VBox);
	this->m_VBox.pack_start(this->m_ScrolledWindow1);
	this->m_ScrolledWindow1.add(this->m_TextView1);
	this->m_refTextBuffer1 = Gtk::TextBuffer::create();
	this->m_refTextBuffer1->set_text("");
	this->m_TextView1.set_buffer(this->m_refTextBuffer1);
	this->m_TextView1.set_editable(false);
	this->m_VBox.pack_start(this->m_HBox);
	this->m_HBox.pack_start(this->m_ScrolledWindow2);
	this->m_ScrolledWindow2.add(this->m_TextView2);
	this->m_refTextBuffer2 = Gtk::TextBuffer::create();
	this->m_refTextBuffer2->set_text("");
	this->m_TextView2.set_buffer(this->m_refTextBuffer2);
	this->m_HBox.pack_start(m_button_send);
	this->m_button_send.set_label("Send");

	this->m_button_send.signal_clicked().connect(sigc::mem_fun(*this, &GameWindow::on_mensaje) );

	show_all_children();
	show_all();
}

GameWindow::~GameWindow(){}

void GameWindow::on_mensaje(){
	string str = this->m_refTextBuffer2->get_text();
	this->m_refTextBuffer2->set_text("");
	Json::Value data;
	data["event"] = EVENT_GAME_MISC;
	data["ev-game"] = EVENT_GAME_CHAT;
	data["line"] = str;
	this->m_signal_mensaje.emit(data);
}

void GameWindow::on_start_game(){
	Json::Value data;
	data["event"] = EVENT_GAME_MISC;
	data["ev-game"] = EVENT_GAME_START;
	this->m_signal_mensaje.emit(data);
}

void GameWindow::on_salir_game(){
	Json::Value data;
	data["event"] = EVENT_LEAVE_GAME;
		this->m_signal_mensaje.emit(data);
}

void GameWindow::mensaje(Json::Value& data){
	StaticString def("");
	int code;
	CommonEvents event = EVENT_NONE;

	if(data.get("event", def).isNumeric())
		event = (CommonEvents) data.get("event", def).asInt();

	if(data.get("code", def).isNumeric())
		code = (CommonEvents) data.get("code", def).asInt();

	switch(event){
		case EVENT_GAME_CHAT:{
			string str = this->m_refTextBuffer1->get_text();
			string line = data["line"].asString();
			this->m_refTextBuffer1->set_text(str+"\n >> "+line);
			break;
		}
		case EVENT_GAME_MSG:{
			string str = this->m_refTextBuffer1->get_text();
			string line = data["msg"].asString();
			this->m_refTextBuffer1->set_text(str+"\n >> "+line);
			break;
		}
		case EVENT_GAME_USER_RM:
		case EVENT_GAME_USER_ADD:{
			string str = this->m_refTextBuffer1->get_text();
			string line = data["line"].asString(); string text = "\n >> Se ";
			if(event == EVENT_GAME_USER_RM)
				text +="des";
			text += "conecto "+data["user"]["user"].asString();
			this->m_refTextBuffer1->set_text(str+text);
			Json::Value msg;
			msg["event"] = EVENT_GAME_MISC;
			msg["ev-game"] = EVENT_GAME_INFO;
			this->m_signal_mensaje.emit(msg);
			break;
		}

		case EVENT_GAME_INFO:{
			this->user_list.clearRows();
			Json::Value users = data["users"];
			if(users.isArray()){
				std::string id;
				long id_n;
				int puntos, nivel;
				for(int i=0; i < users.size(); i++){
					id = users[i]["id"].asString();
					stringstream ss(id);
					ss >> id_n;
					id = users[i]["user"].asString();
					puntos = users[i]["puntos"].asInt();
					nivel = users[i]["nivel"].asInt();
					this->user_list.addRow(id_n, id, nivel, puntos);
				}
			}
			break;
		}

		case EVENT_GAME_START:{
			// TODO: mover esto a cliente.cliente.cpp y hacer que se borre la ventana de game
			std::cout << "Tableor que recibi  : " << data["tablero"] << std::endl;  
			tableroJuego = new TableroJuego(data["tablero"]);
			tableroJuego->signal_mensaje().connect(sigc::mem_fun(this, &GameWindow::on_tablero_mensaje));
			break;
	   }

		default:
			break;
	}
}

void GameWindow::on_tablero_mensaje(Json::Value data){
	this->m_signal_mensaje.emit(data);
}

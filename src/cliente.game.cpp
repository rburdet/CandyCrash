#include "cliente.game.h"
#include "common.events.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;
using Json::StaticString;
using Json::Value;

GameWindow::GameWindow() {
	this->add(this->m_VBox);
	this->m_VBox.pack_start(this->m_ScrolledWindow1);
	this->m_ScrolledWindow1.add(this->m_TextView1);
	this->m_refTextBuffer1 = Gtk::TextBuffer::create();
	this->m_refTextBuffer1->set_text("");
	this->m_TextView1.set_buffer(this->m_refTextBuffer1);
	this->m_TextView1.set_editable(false);
	this->m_VBox.pack_start(this->m_ScrolledWindow2);
	this->m_ScrolledWindow2.add(this->m_TextView2);
	this->m_refTextBuffer2 = Gtk::TextBuffer::create();
	this->m_refTextBuffer2->set_text("");
	this->m_TextView2.set_buffer(this->m_refTextBuffer2);
	this->m_VBox.pack_start(m_button_send);
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
		case EVENT_GAME_USER_ADD:{
			string str = this->m_refTextBuffer1->get_text();
			string line = data["line"].asString();
			this->m_refTextBuffer1->set_text(str+"\n >> Se conecto "+data["user"]["user"].asString());
			break;
		}

		case EVENT_GAME_USER_RM:{
			 break;
		}

		default:
			break;
	}
}

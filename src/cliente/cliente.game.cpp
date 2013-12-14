#include "cliente.game.h"
#include "../common/common.events.h"
#include <string>
#include <sstream>
#include "cliente.sound_player.h"

using std::string;
using std::stringstream;
using Json::StaticString;
using Json::Value;

GameWindow::GameWindow() {
	this->set_title("Juego");
	this->set_size_request(350, 350);

	this->tableroJuego = NULL;

	this->add(this->m_VBox);

	m_VBox.pack_start(menubar, Gtk::PACK_SHRINK, 0);
	menubar.signal_quit().connect(sigc::mem_fun(*this, &GameWindow::on_salir));
	menubar.signal_disconnect().connect(sigc::mem_fun(*this, 
				&GameWindow::on_desconectar));
	menubar.signal_help().connect(sigc::mem_fun(*this,&GameWindow::on_help));
	menubar.signal_about().connect(sigc::mem_fun(*this,&Window::on_about));

	m_VBox.pack_start(padBox, true, true, 10);
	padBox.pack_start(this->mainV, true, true, 5);

	this->mainV.pack_start(this->user_list, true, true, 10);
	Glib::RefPtr<Gtk::StyleContext> stylecontext = 
		this->user_list.get_style_context();
	stylecontext->add_class("user_list");
	stylecontext->context_save();

	this->mainV.pack_start(this->m_ScrolledWindow1, true, true, 0);
	this->m_ScrolledWindow1.set_policy(Gtk::POLICY_AUTOMATIC,
			Gtk::POLICY_AUTOMATIC);
	this->m_ScrolledWindow1.add(this->m_TextView1);
	this->m_TextView1.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
	this->m_refTextBuffer1 = Gtk::TextBuffer::create();
	this->m_refTextBuffer1->set_text("");
	this->m_TextView1.set_buffer(this->m_refTextBuffer1);
	this->m_TextView1.set_editable(false);
	this->mainV.pack_start(this->m_HBox, false, false, 0);
	this->m_HBox.pack_start(this->text_input, true, true, 0);
	this->text_input.signal_activate().connect(sigc::mem_fun(*this, 
				&GameWindow::on_mensaje) );
	this->m_HBox.pack_start(m_button_send, true, true, 0);
	this->m_button_send.set_label("Enviar");
	this->m_button_send.signal_clicked().connect(sigc::mem_fun(*this, 
				&GameWindow::on_mensaje) );


	stylecontext = this->m_ScrolledWindow1.get_style_context();
	stylecontext->add_class("scrolled-text");
	stylecontext->context_save();

	stylecontext = this->m_TextView1.get_style_context();
	stylecontext->add_class("textview");
	stylecontext->context_save();

	stylecontext = this->text_input.get_style_context();
	stylecontext->add_class("chat_entry");
	stylecontext->context_save();

	stylecontext = m_button_send.get_style_context();
	stylecontext->add_class("btn");
	stylecontext->context_save();

	this->mainV.pack_start(this->but_hbox, false, false, 10);
	this->but_hbox.pack_start(this->button_start, true, true, 10);
	this->button_start.set_label("Empezar el juego");
	this->button_start.signal_clicked().connect(sigc::mem_fun(*this, 
				&GameWindow::on_start_game) );
	this->but_hbox.pack_start(this->button_salir, true, true, 10);
	this->button_salir.set_label("Salir de la partida");
	this->button_salir.signal_clicked().connect(sigc::mem_fun(*this, 
				&GameWindow::on_salir_game) );


	stylecontext = this->button_start.get_style_context();
	stylecontext->add_class("btn");
	stylecontext->context_save();

	stylecontext = this->button_salir.get_style_context();
	stylecontext->add_class("btn1");
	stylecontext->context_save();

	show_all_children();
	show_all();
}

GameWindow::~GameWindow(){
	if(tableroJuego){
		tableroJuego->hide();
		delete tableroJuego;
		tableroJuego = NULL;
	}
}

void GameWindow::on_mensaje(){
	string str = this->text_input.get_text();
	this->text_input.set_text("");
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
	if(tableroJuego){
		tableroJuego->hide();
		delete tableroJuego;
		tableroJuego = NULL;
	}
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
			Gtk::TextIter it = this->m_refTextBuffer1->end();
			string line = data["line"].asString();
			this->m_refTextBuffer1->insert(it, "\n >> "+line);
			Glib::RefPtr< Gtk::TextBuffer::Mark > mark = 
				this->m_refTextBuffer1->get_insert();
			this->m_TextView1.scroll_to(mark);
			SoundPlayer::play("../share/candycrash/sounds/message-new-instant.wav");
			break;
		}
		case EVENT_GAME_MSG:{
			Gtk::TextIter it = this->m_refTextBuffer1->end();
			string line = data["msg"].asString();
			this->m_refTextBuffer1->insert(it, "\n >> "+line);
			Glib::RefPtr< Gtk::TextBuffer::Mark > mark = 
				this->m_refTextBuffer1->get_insert();
			this->m_TextView1.scroll_to(mark);
			break;
		}
		case EVENT_GAME_USER_RM:
		case EVENT_GAME_USER_ADD:{
			Gtk::TextIter it = this->m_refTextBuffer1->end();
			string line = data["line"].asString(); string text = "\n >> Se ";
			if(event == EVENT_GAME_USER_RM){
				SoundPlayer::play("../share/candycrash/sounds/device-removed.wav");
				text +="des";
			}else{
				SoundPlayer::play("../share/candycrash/sounds/device-added.wav");
			}
			text += "conecto "+data["user"]["user"].asString();
			this->m_refTextBuffer1->insert(it, text);
			Glib::RefPtr< Gtk::TextBuffer::Mark > mark = 
				this->m_refTextBuffer1->get_insert();
			this->m_TextView1.scroll_to(mark);
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
			tableroJuego = new TableroJuego(data["tablero"]);
			tableroJuego->signal_mensaje().connect(sigc::mem_fun(this, 
						&GameWindow::on_tablero_mensaje));
			break;
		}

		case EVENT_GAME_MOV:{
			Json::Value msg;
			msg["event"] = EVENT_GAME_MISC;
			msg["ev-game"] = EVENT_GAME_INFO;
			this->m_signal_mensaje.emit(msg);

			this->tableroJuego->mensaje(data);
			break;
		}

		case EVENT_GAME_END:{
			//tableroJuego->hide();
			//delete tableroJuego;
			//tableroJuego = NULL;

			string msg = data["msg"].asString();
			Gtk::TextIter it = this->m_refTextBuffer1->end();
			this->m_refTextBuffer1->insert(it, "\n >> Termino la partida\n>>"+msg);
			Glib::RefPtr< Gtk::TextBuffer::Mark > mark = 
				this->m_refTextBuffer1->get_insert();
			this->m_TextView1.scroll_to(mark);

			SoundPlayer::play("../share/candycrash/sounds/complete.wav");
			break;
		}

		default:
			break;
	}
}

void GameWindow::on_tablero_mensaje(Json::Value data){
	this->m_signal_mensaje.emit(data);
}

bool GameWindow::onClose(){
	//TODO: tildes
	if(this->dialog("Desea Cerrar?", "No podra seguir jugando")){
		this->hide();
		Gtk::Main::quit();
	}

	return true;
}

void GameWindow::on_salir(){
	this->onClose();
}

void GameWindow::on_desconectar(){
	if(this->dialog("Esta seguro de desconectarse?", "No podra seguir jugando")){
		Json::Value fake;
		fake["event"] = EVENT_LOGOUT;
		fake["msj"] = "";
		m_signal_mensaje.emit(fake);
	}
}

void GameWindow::on_help(){
	Gtk::MessageDialog dialog(*this, "Ayuda");
	dialog.set_secondary_text("En esta instancia Ud ya esta conectado y"
			"esperando que empiece la partida. \n"
		"Aqui puede chatear con los demas jugadores conectado a esta misma "
		"partida, en ningun caso Ud podra ver la identidad de quien escribe.\n"
		"Solo el creador de la partida puede dar comienzo a la misma mediante"
		" el boton de \" Empezar el juego \" .\n "
		"En caso de querer abandonar la partida presione "
		"\"Salir de la partida\".\n\n"
		"Aclaracion: Si el creador de la partida abandona la partida, cualquiera"
		" de los jugadores puede dar comienzo.");
		dialog.run();
}

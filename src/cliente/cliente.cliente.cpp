#include "cliente.cliente.h"
#include "../common/common.events.h"
#include "../common/common.logger.h"
#include "cliente.main_window.h"
#include "cliente.game.h"
#include <sstream>
#include <string>
#include <vector>
#include "cliente.sound_player.h"

using std::string;
using Json::Value;
using Json::StaticString;
using std::stringstream;

Cliente::Cliente() : ventanaActual(NULL), listener(NULL){
	sigc::slot<bool> my_slot = sigc::mem_fun(*this, &Cliente::onTimeout);

	this->connTimeout = Glib::signal_timeout().connect(my_slot, 100);

	// Agregamos el estilo
	try {
		Glib::RefPtr< Gdk::Screen > screen = Gdk::Screen::get_default();
		Glib::RefPtr<Gtk::CssProvider> cssprov = Gtk::CssProvider::create();
		cssprov->load_from_path("../share/candycrash/imagenes/style.css");
		Gtk::StyleContext::add_provider_for_screen(screen, cssprov,
				GTK_STYLE_PROVIDER_PRIORITY_USER);
	} catch(Glib::Error err){
		Logger::log("Error parseando el css: '"+err.what()+"'");
	}

	Glib::RefPtr< Gdk::Pixbuf > icon = 
		Gdk::Pixbuf::create_from_file("../share/candycrash/imagenes/star.png");
	std::vector< Glib::RefPtr< Gdk::Pixbuf > > icon_list;
	icon_list.push_back(icon);
	Gtk::Window::set_default_icon_list(icon_list);
}

void Cliente::mostrarVentanaIP(){
	Ipwindow* ventana = new Ipwindow();
	ventanaActual = ventana;

	ventana->signal_conectar().connect(sigc::mem_fun(this, &Cliente::conectar));

}

void Cliente::conectar(string ip, string user, string pass, bool check){
	TCPSocketConnect* sd = new TCPSocketConnect;
	if(sd->connect(ip) != 0){
		Json::Value val;
		val["msj"] = "Servidor inexistente!";
		//Ipwindow* ventana = this->ventanaActual;
		//ventana->set_editable(true);
		this->ventanaActual->mensaje(val);
		delete sd;
		return;
	}

	this->listener = new ThreadListener(this, sd);
	this->listener->setKey(pass);
	this->listener->start();
	Json::Value sendMsj;
	sendMsj["user"] = user;
	sendMsj["pass"] = pass;
	sendMsj["event"] = check ? EVENT_NEW_USER : EVENT_LOGIN;
	this->listener->write(sendMsj);
}

Cliente::~Cliente(){
	if(ventanaActual)
		delete ventanaActual;
}

void Cliente::nuevoMensaje(Json::Value& msj){
	this->mensajesMutex.lock();
	this->mensajes.push(msj);
	this->mensajesMutex.unlock();
	//std::cout << msj << std::endl;
}

bool Cliente::onTimeout(){
	Json::Value data;

	this->mensajesMutex.lock();
	if(this->mensajes.empty()){
		this->mensajesMutex.unlock();
			return true;
		}
		data = this->mensajes.front();
		this->mensajes.pop();
		this->mensajesMutex.unlock();

		// TODO: se podria pasar a una funcion
		StaticString def("");
		CommonEvents event = EVENT_NONE;
		int code = -1;

		if(data.get("event", def).isNumeric())
			event = (CommonEvents) data.get("event", def).asInt();

		if(data.get("code", def).isNumeric())
			code = (CommonEvents) data.get("code", def).asInt();

		//Json::StyledWriter writer;
		//string output = writer.write(data);
		//std::cout << output << std::endl;

		switch(event){
			case EVENT_LOGIN:
				this->onLogin(code, data);
				break;

			case EVENT_LOGOUT:
				this->onLogout(code, data);
				break;

			case EVENT_NEW_USER:
				this->ventanaActual->mensaje(data);
				break;

			case EVENT_JOIN_GAME:
			case EVENT_NEW_GAME:
				this->onGame(code, data);
				break;

			case EVENT_LEAVE_GAME:{
				int root_x, root_y;
				ventanaActual->get_position(root_x, root_y);
				ventanaActual->hide();
				delete ventanaActual;
				MainWindow* win = new MainWindow;
				win->move(root_x, root_y);
				win->signal_mensaje().connect(sigc::mem_fun(this, 
							&Cliente::sendMsj));
				win->on_partidas();
				win->on_mapas();
				this->ventanaActual = win;
				break;
			}

			case EVENT_LIST_GAMES:
			default:
				Logger::log("Default. Redirecciono a ventana");
				this->ventanaActual->mensaje(data);
			break;
	}

	return true;
}

void Cliente::sendMsj(Json::Value data){
	if(data["event"] == EVENT_LOGOUT){
		this->onLogout(0, data);
		return;
	}

	this->listener->write(data);
}

void Cliente::onLogout(int code, Json::Value& data){
	if(!code)
		this->listener->shutdown();

	if(this->listener){
		this->listener->join();
		this->listener = NULL;
	}
	if(ventanaActual){
		ventanaActual->hide();
		delete ventanaActual;
		ventanaActual = NULL;
	}
	this->mostrarVentanaIP();
	this->ventanaActual->mensaje(data);
}
void Cliente::onLogin(int code, Json::Value& data){
	if(!code){
		this->userData = data["user"];
		int root_x, root_y;
		ventanaActual->get_position(root_x, root_y);
		ventanaActual->hide();
		delete ventanaActual;
		SoundPlayer::play("../share/candycrash/sounds/service-login.wav");
		MainWindow* ventana = new MainWindow;
		ventana->signal_mensaje().connect(sigc::mem_fun(this, &Cliente::sendMsj));
		ventana->move(root_x, root_y);
		ventana->on_partidas();
		ventana->on_mapas();
		this->ventanaActual = ventana;
	}else{
		this->listener->shutdown();
		this->listener->join();
		this->listener = NULL;
		//this->listener->join();
		//delete this->listener;
		//this->listener = NULL;

		this->ventanaActual->mensaje(data);
	}
}

void Cliente::onGame(int code, Json::Value& data){
	if(code){
		this->ventanaActual->mensaje(data);
	}else{
		int root_x, root_y;
		ventanaActual->get_position(root_x, root_y);
		this->ventanaActual->hide();
		delete this->ventanaActual;

		GameWindow* gwin = new GameWindow;
		this->ventanaActual = gwin;
		this->ventanaActual->move(root_x, root_y);
		gwin->signal_mensaje().connect(sigc::mem_fun(this, &Cliente::sendMsj));

		Json::Value msj;
		msj["event"] = EVENT_GAME_MISC;
		msj["ev-game"] = EVENT_GAME_INFO;
		this->sendMsj(msj);
	}
}


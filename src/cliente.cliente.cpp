#include "cliente.cliente.h"
#include "common.events.h"
#include "common.logger.h"
#include "cliente.main_window.h"
#include <sstream>

using std::string;
using Json::Value;
using Json::StaticString;
using std::stringstream;

Cliente::Cliente() : ventanaActual(NULL), listener(NULL){
	sigc::slot<bool> my_slot = sigc::mem_fun(*this, &Cliente::onTimeout);

	this->connTimeout = Glib::signal_timeout().connect(my_slot, 100);
}

void Cliente::mostrarVentanaIP(){
	Ipwindow* ventana = new Ipwindow();
	ventanaActual = ventana;

	ventana->signal_conectar().connect(sigc::mem_fun(this, &Cliente::conectar));
}

void Cliente::conectar(string ip, string user, string pass, bool check){
	// TODO: check errores
	std::cout << "Me estoy conectado: '" << ip << "' user:'" << user << "' pass: '" << pass << "'" << std::endl;

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

Cliente::~Cliente(){ }

void Cliente::nuevoMensaje(Json::Value& msj){
	this->mensajesMutex.lock();
	this->mensajes.push(msj);
	this->mensajesMutex.unlock();
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

	switch(event){
		case EVENT_LOGIN:
			this->onLogin(code, data);
			break;

		case EVENT_NEW_USER:
			this->ventanaActual->mensaje(data);
			break;

		case EVENT_LIST_GAMES:
		default:
			Logger::log("No se que evento me pasaste");
			this->ventanaActual->mensaje(data);
			break;
	}

	return true;
}

void Cliente::sendMsj(Json::Value data){
	//Json::Reader reader;
	//Json::Value data;
	//if(!reader.parse(str, data, false)){
	//	Logger::log("error parseando json");
	//	return;
	//}

	this->listener->write(data);
}

void Cliente::onLogin(int code, Json::Value& data){
	if(!code){
		this->userData = data["user"];
		std::cout << this->userData["nivel"].asInt() << std::endl;
		ventanaActual->hide();
		delete ventanaActual;
		MainWindow* ventana = new MainWindow;
		this->ventanaActual = ventana;
		ventana->signal_mensaje().connect(sigc::mem_fun(this, &Cliente::sendMsj));
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

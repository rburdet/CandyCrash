#include "cliente.cliente.h"
#include "common.events.h"
#include "common.logger.h"
#include "cliente.main_window.h"

using std::string;
using Json::Value;
using Json::StaticString;

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
		string str = "Servidor inexistente!";
		Ipwindow* ventana = this->ventanaActual;
		ventana->set_editable(true);
		ventana->set_text(str);
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

	Json::StyledWriter writer;
	string output = writer.write(data);
	Logger::log(output);

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

		case EVENT_NEW_USER:{
			Ipwindow* ventana = this->ventanaActual;
			ventana->set_editable(true);
			string str;
			if(!code){
				str = "Usuario creado. Conectese";
				ventana->set_text(str);
			}else{
				str = "Error";
				ventana->set_text(str);
			}

			break;
		}

		default:{
			Logger::log("No se que evento me pasaste");
			MainWindow* ventana = (MainWindow*) this->ventanaActual;
			ventana->setText(output);
			break;
		}
	}

	return true;
}

void Cliente::sendMsj(std::string str){
	Json::Reader reader;
	Json::Value data;
	if(!reader.parse(str, data, false)){
		Logger::log("error parseando json");
		return;
	}

	this->listener->write(data);
}

void Cliente::onLogin(int code, Json::Value& data){
	if(!code){
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

		string str = "Error login";
		Ipwindow* ventana = this->ventanaActual;
		ventana->set_editable(true);
		ventana->set_text(str);
	}
}
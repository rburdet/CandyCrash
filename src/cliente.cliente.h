#ifndef CLIENT_CLIENTE_H
#define CLIENT_CLIENTE_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <queue>
#include "cliente.socket_connect.h"
#include "cliente.ipwindow.h"
#include "cliente.cliente_interface.h"
#include "cliente.thread_listener.h"
#include "common.mutex.h"

class Cliente : public ClienteInterface {
	public: 
		Cliente();
		~Cliente();
		void mostrarVentanaIP();
		void sendMsj(std::string str);
		void conectar(std::string ip, std::string user, std::string pass, bool check);
		virtual void nuevoMensaje(Json::Value& msj);
	private:
		Gtk::Window* ventanaActual;
		ThreadListener *listener;
		std::queue<Json::Value> mensajes;
		Mutex mensajesMutex;
		sigc::connection connTimeout;
		bool onTimeout();

		void onLogin(int code, Json::Value& data);
};

#endif

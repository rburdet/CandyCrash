#ifndef CLIENT_CLIENTE_H
#define CLIENT_CLIENTE_H

#include <gtkmm.h>
#include <string>
#include "cliente.socket_connect.h"
#include "cliente.ipwindow.h"

class Cliente{
	public: 
		Cliente();
		~Cliente();
		void mostrarVentanaIP();
		void conectar(std::string ip);
	private:
		TCPSocketConnect sd;
		Gtk::Window* ventanaActual;
};

#endif

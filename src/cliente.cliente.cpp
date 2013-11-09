#include "cliente.cliente.h"

using std::string;

Cliente::Cliente() : ventanaActual(NULL) {
}

void Cliente::mostrarVentanaIP(){
	Ipwindow* ventana = new Ipwindow();
	ventanaActual = ventana;

	ventana->signal_conectar().connect(sigc::mem_fun(this, &Cliente::conectar));
}

void Cliente::conectar(string ip){
	// TODO: check errores
	sd.connect(ip);
	std::cout << "Me estoy conectado" << ip << std::endl;
}

Cliente::~Cliente(){ }


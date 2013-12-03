#include <gtkmm/main.h>
#include "common.logger.h"
#include "cliente.cliente.h"

int main(int argc , char* argv[]){
	Gtk::Main kit(argc,argv);
	Logger::init();
	Cliente cliente;
	cliente.mostrarVentanaIP();
	Gtk::Main::run();
	Logger::destroy();
	return 0;
}

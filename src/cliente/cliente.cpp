#include <gtkmm/main.h>
#include "../common/common.logger.h"
#include "../common/common.path_manager.h"
#include "cliente/cliente.cliente.h"

int main(int argc , char* argv[]){
	Gtk::Main kit(argc,argv);
	Logger::init();
	PathManager::init();
	PathManager::chdir();
	Cliente cliente;
	cliente.mostrarVentanaIP();
	Gtk::Main::run();
	Logger::destroy();
	PathManager::destroy();
	return 0;
}

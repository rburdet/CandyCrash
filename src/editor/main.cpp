#include "mainwindow.h"
#include <gtkmm/main.h>
#include <iostream>
#include "../common/common.path_manager.h"
#include "../common/common.logger.h"

int main(int argc, char *argv[]){
	PathManager::init();
	PathManager::chdir();
	Logger::init();
	Gtk::Main kit(argc,argv);
	MainWindow window;
	kit.run(*(window.getMainWindow()));
	Logger::destroy();
	return 0;
}

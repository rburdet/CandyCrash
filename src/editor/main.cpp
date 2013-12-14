#include "mainwindow.h"
#include <gtkmm/main.h>
#include <iostream>
#include "../common/common.path_manager.h"

int main(int argc, char *argv[]){
	PathManager::init();
	PathManager::chdir();
	Gtk::Main kit(argc,argv);
	MainWindow window;
	kit.run(*(window.getMainWindow()));
	return 0;
}

#include "mainwindow.h"
#include <gtkmm/main.h>
#include <iostream>

int main(int argc, char *argv[]){
	Gtk::Main kit(argc,argv);
	MainWindow window;
	kit.run(*(window.getMainWindow()));
	return 0;
}

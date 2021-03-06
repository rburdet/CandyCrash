#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include "mainwindow.h"
#include "probceldas.h"
#include "probcolumnas.h"
#include <iostream>
#include "menupersistidor.h"


MainWindow::MainWindow(){
	this->builder = Gtk::Builder::create_from_file("../share/candycrash/editor/Editor.glade");
	this->builder->get_widget("window1",mainWindow);
	this->tablero = new Tablero(this->builder);
	Sonidos* sonidos = new Sonidos(this->builder);
	this->boxOpcionesBasicas = new BoxOpcionesBasicas(this->builder,this->tablero,sonidos);
	this->menu = new Menu(this->builder);
	builder->get_widget("quit",close);
	ProbColumnas probColumnas(this->tablero,this->builder);
	ProbCeldas probCeldas(this->tablero,this->builder); 
	this->menuPersistidor = new MenuPersistidor(this->boxOpcionesBasicas,this->builder);
	close->signal_activate().connect(sigc::mem_fun(*this,&MainWindow::cerrar));
}

Gtk::Window* MainWindow::getMainWindow(){
	return this->mainWindow;
}

void MainWindow::cerrar(){
	Gtk::Window* window = new Window();
	Gtk::MessageDialog dialog(*window,"Desea cerrar?",false,
			Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
	dialog.set_secondary_text("Asegurese de generar antes de salir");
	int opc=dialog.run();
	if (opc==Gtk::RESPONSE_OK)
		this->mainWindow->hide();
}

MainWindow::~MainWindow(){
	delete boxOpcionesBasicas;
	delete tablero;
}


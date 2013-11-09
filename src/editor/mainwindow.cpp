#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include "mainwindow.h"
#include "probceldas.h"
#include "probcolumnas.h"
#include <iostream>


MainWindow::MainWindow(){
	this->builder = Gtk::Builder::create_from_file("Editor.glade");
	this->builder->get_widget("window1",mainWindow);
	this->tablero = new Tablero(this->builder);
	this->boxOpcionesBasicas = new BoxOpcionesBasicas(this->builder,this->tablero);
	ProbColumnas probColumnas(this->tablero,this->builder);
	ProbCeldas probCeldas(this->tablero,this->builder);
}

Gtk::Window* MainWindow::getMainWindow(){
	return this->mainWindow;
}

MainWindow::~MainWindow(){
	delete boxOpcionesBasicas;
}


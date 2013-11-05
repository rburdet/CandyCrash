#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include "mainwindow.h"
#include <iostream>


MainWindow::MainWindow(){
	this->builder = Gtk::Builder::create_from_file("Editor.glade");
	this->builder->get_widget("window1",mainWindow);
	this->tablero = new Tablero(this->builder);
	this->boxOpcionesBasicas = new BoxOpcionesBasicas(this->builder,this->tablero);
	
	//this->builder->get_widget("e_maxjug",s_maxjug);
	//this->builder->get_widget("s_puntaje",s_puntaje);
	//this->builder->get_widget("e_nombre",e_nombre);
	//this->builder->get_widget("button_json",button_json);
	//button_json->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_button_clicked));
}

Gtk::Window* MainWindow::getMainWindow(){
	return this->mainWindow;
}

//Gtk::Fixed* MainWindow::getTablero(){
//	return this->tablero;
//}

//
//MainWindow::MainWindow()
//: m_table(6, 4),
//  m_button("JSonear"),
//  label_nombre("nombre:"),
//  label_puntaje("puntaje:"),
//  label_maxjug("jugadores:"),
//  label_dimx("X:"),
//  label_dimy("Y:")
//{
//
//  set_title("Gtk::Assistant example");
//  set_border_width(12);
//  m_table.attach(m_button, 0, 5, 0, 1, Gtk::FILL, Gtk::FILL);
//  m_table.attach(label_nombre, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL);
//  m_table.attach(label_puntaje, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL);
//  m_table.attach(label_maxjug, 0, 1, 3, 4, Gtk::FILL, Gtk::FILL);
//  label_dimx.set_width_chars(2);
//  m_table.attach(label_dimx, 0, 1, 4, 5);
//  m_table.attach(label_dimy, 2, 3, 4, 5);
//  m_table.attach(m_entry_nombre, 1, 5, 1, 2);
//  m_table.attach(m_entry_puntaje, 1, 5, 2, 3);
//  m_table.attach(m_entry_maxjug, 1, 5, 3, 4);
//  m_table.attach(m_entry_dimx, 1, 2, 4, 5,Gtk::SHRINK);
//  m_table.attach(m_entry_dimy, 4, 5, 4, 5,Gtk::SHRINK);
//  add(m_table);
//
//  label_nombre.set_alignment(0.0, 0.5);
//  label_puntaje.set_alignment(0.0, 0.5);
//  label_maxjug.set_alignment(0.0, 0.5);
//  label_dimx.set_alignment(0.0, 0.5);
//  label_dimy.set_alignment(0.0, 0.5);
//
//  m_button.signal_clicked().connect(sigc::mem_fun(*this,
//    &MainWindow::on_button_clicked));
//
//  m_check.set_sensitive(true);
//  m_entry_nombre.set_sensitive(true);
//  m_entry_puntaje.set_sensitive(true);
//  m_entry_maxjug.set_sensitive(true);
//  m_entry_dimx.set_sensitive(true);
//  m_entry_dimy.set_sensitive(true);
//
//  show_all_children();
//}

MainWindow::~MainWindow(){
	delete boxOpcionesBasicas;
	delete noteProb;
}


//std::string MainWindow::getNombre(){
//	return (e_nombre->get_text());
//}
//
//double MainWindow::getPuntaje(){
//	return (s_puntaje->get_value());
//}
//
//double MainWindow::getJugadoresMax(){
//	return (s_maxjug->get_value());
//}

//std::string MainWindow::getDimX(){
//	return (m_entry_dimx.get_text());
//}

//std::string MainWindow::getDimY(){
//	return (m_entry_dimy.get_text());
//}

//void MainWindow::on_button_clicked(){
//	Json::Value nombre_nivel = getNombre();
//	Json::Value puntaje_para_ganar = (int)getPuntaje();
//	Json::Value max_jugadores = (int)getJugadoresMax();
//	Json::Value nivel;
//	//Json::Value imagen_fondo = getImagenFondo();
//	//Json::Value filas = getNFilas();
//	//Json::Value columnas;
//	nivel["nombre_nivel"]=nombre_nivel;
//	nivel["puntaje_para_ganar"]=puntaje_para_ganar;
//	nivel["max_jugadores"]=max_jugadores;
//	std::cout<<nivel<<std::endl;
//}

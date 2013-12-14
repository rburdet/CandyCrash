#include "generalprobs.h"
#include <vector>
#include <string>

#define DEFAULTIMAGEDIR "../share/candycrash/imagenes/"
#define WIDGETNAME "image"
#define BUTTON "spinbutton"
#define ADJUSTMENT "adjustment"
#include <iostream>

Probs::Probs(Tablero* tablero, Glib::RefPtr<Gtk::Builder>& builder, 
		int startNumber , int finalNumber,std::string frameName){
	std::vector<std::string> nombres;
	this->tablero=tablero;
	std::string str;
	builder->get_widget(frameName,aFrame);
	//Creo un vector de 16 imagenes ( con N del 1 al 16 o del 17 al 32)
	for (int i=startNumber ; i <= finalNumber ; i++){
		std::stringstream ss;
		Gtk::Image * aux;
		ss << WIDGETNAME << i; 
		builder->get_widget(ss.str(),aux);
		candys.push_back(aux);
	}

	std::string IMAGEDIR(DEFAULTIMAGEDIR);

	nombres.push_back(IMAGEDIR+"button_red.png");
	nombres.push_back(IMAGEDIR+"button_blue.png");
	nombres.push_back(IMAGEDIR+"button_green.png");
	nombres.push_back(IMAGEDIR+"button_purple.png");
	nombres.push_back(IMAGEDIR+"button_yellow.png");
	nombres.push_back(IMAGEDIR+"bar_hor_red.png");
	nombres.push_back(IMAGEDIR+"bar_hor_blue.png");
	nombres.push_back(IMAGEDIR+"bar_hor_green.png");
	nombres.push_back(IMAGEDIR+"bar_hor_purple.png");
	nombres.push_back(IMAGEDIR+"bar_hor_yellow.png");
	nombres.push_back(IMAGEDIR+"bar_vert_red.png");
	nombres.push_back(IMAGEDIR+"bar_vert_blue.png");    	
	nombres.push_back(IMAGEDIR+"bar_vert_green.png");
	nombres.push_back(IMAGEDIR+"bar_vert_purple.png");
	nombres.push_back(IMAGEDIR+"bar_vert_yellow.png");
	nombres.push_back(IMAGEDIR+"star.png");
	
	//Seteo las imagenes
	for ( int i = 0 ; i <= 15 ; i++ ){
		candys[i]->set(nombres[i]);
	}
}

void Probs::cargarBotones(Glib::RefPtr<Gtk::Builder>& builder,int primerBoton,
		int ultimoBoton){
	for ( int i = primerBoton ; i <= ultimoBoton ; i++ ){
		std::stringstream ss;
		Gtk::SpinButton* aux;
		Glib::RefPtr< Gtk::Adjustment > aux2;
		ss << BUTTON << i; 
		builder->get_widget(ss.str(),aux);
		spinbuttons.push_back(aux);
		std::stringstream ss2;
		ss2 << ADJUSTMENT << i;
		aux2= Glib::RefPtr<Gtk::Adjustment>::cast_dynamic
			(builder->get_object(ss2.str()));
		adjustments.push_back(aux2);
	}
}

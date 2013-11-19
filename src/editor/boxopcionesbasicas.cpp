#include "boxopcionesbasicas.h"

#define MIN_COLS 8
#define MIN_FILAS 8


BoxOpcionesBasicas::BoxOpcionesBasicas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero){
	builder->get_widget("e_maxjug",s_maxjug);
	builder->get_widget("s_puntaje",s_puntaje);
	builder->get_widget("e_nombre",e_nombre);
	builder->get_widget("button_json",button_json);
	builder->get_widget("spin_x",spin_x);
	builder->get_widget("spin_y",spin_y);
	builder->get_widget("spinbuttonnivel",spin_nivel);
	this->tablero=tablero;
	Glib::RefPtr<Gtk::Adjustment> adjustment_cordx = 
	Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment_cordx"));
	Glib::RefPtr<Gtk::Adjustment> adjustment_cordy = 
	Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment_cordy"));
	Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment_nivel"));

	button_json->signal_clicked().connect(sigc::mem_fun(*this, &BoxOpcionesBasicas::on_button_clicked));

	//Senales para spin button, cuando cambia le avisa al tablero que cambio y le informa sobre su nuevo valor
	adjustment_cordx->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero,&Tablero::on_cordx_changed),spin_x));
	adjustment_cordy->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero,&Tablero::on_cordy_changed),spin_y));
}

std::string BoxOpcionesBasicas::getNombre(){
	return (e_nombre->get_text());
}

double BoxOpcionesBasicas::getPuntaje(){
	return (s_puntaje->get_value());
}

double BoxOpcionesBasicas::getJugadoresMax(){
	return (s_maxjug->get_value());
}

int BoxOpcionesBasicas::getDimX(){
	return (spin_x->get_value_as_int());
}

int BoxOpcionesBasicas::getDimY(){
	return (spin_y->get_value_as_int());
}

int BoxOpcionesBasicas::getNivel(){
	return (spin_nivel->get_value_as_int());
}

void BoxOpcionesBasicas::on_button_clicked(){

	const std::string& nombre = getNombre();
	Json::Value puntaje_para_ganar = (int)getPuntaje();
	Json::Value max_jugadores = (int)getJugadoresMax();
	Json::Value nivel;
	Json::Value nivelMax = getNivel();
	Json::Value dimX = MIN_FILAS;
	Json::Value dimY = MIN_COLS;
	if (getDimX() >= MIN_FILAS)
		dimX = getDimX();
	if (getDimY() >= MIN_COLS)
		dimY= getDimY();
	this->tablero->jsonCeldas(nivel,nombre);
	this->tablero->jsonColumnas(nivel,nombre);
	nivel[nombre]["puntaje_para_ganar"]=puntaje_para_ganar;
	nivel[nombre]["max_jugadores"]=max_jugadores;
	nivel[nombre]["nivel"] = nivelMax;
	nivel[nombre]["DIM"]["X"] = dimX;
	nivel[nombre]["DIM"]["Y"] = dimY;
	Persistidor::persistir(nivel,nombre);
	//std::cout<<nivel<<std::endl;
}

BoxOpcionesBasicas::~BoxOpcionesBasicas(){}

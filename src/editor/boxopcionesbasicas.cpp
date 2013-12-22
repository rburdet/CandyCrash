#include "boxopcionesbasicas.h"
#include <string>

#define MIN_COLS 8
#define MIN_FILAS 8
#define MAPA_PATH "../share/candycrash/Mapas/"


BoxOpcionesBasicas::BoxOpcionesBasicas(Glib::RefPtr<Gtk::Builder>& builder,
		Tablero* tablero){
	builder->get_widget("e_maxjug",s_maxjug);
	builder->get_widget("s_puntaje",s_puntaje);
	builder->get_widget("e_nombre",e_nombre);
	builder->get_widget("button_json",button_json);
	builder->get_widget("spin_x",spin_x);
	builder->get_widget("spin_y",spin_y);
	builder->get_widget("spinbuttonnivel",spin_nivel);
	this->barraEstado = new BarraEstado(builder);
	this->tablero=tablero;
	Glib::RefPtr<Gtk::Adjustment> adjustment_cordx = 
	Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object
			("adjustment_cordx"));
	Glib::RefPtr<Gtk::Adjustment> adjustment_cordy = 
	Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object
			("adjustment_cordy"));
	Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object
			("adjustment_nivel"));

	button_json->signal_clicked().connect(sigc::mem_fun
			(*this, &BoxOpcionesBasicas::on_button_clicked));
	button_json->signal_clicked().connect(sigc::bind(sigc::mem_fun
				(barraEstado, &BarraEstado::onMostrar),e_nombre));

	//Senales para spin button, cuando cambia le avisa al tablero que cambio 
	//y le informa sobre su nuevo valor
	adjustment_cordx->signal_value_changed().connect(sigc::bind(sigc::mem_fun
				(tablero,&Tablero::on_cordx_changed),spin_x));
	adjustment_cordy->signal_value_changed().connect(sigc::bind(sigc::mem_fun
				(tablero,&Tablero::on_cordy_changed),spin_y));
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
	this->generar();
}

void BoxOpcionesBasicas::generar(){
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
}

BoxOpcionesBasicas::~BoxOpcionesBasicas(){
	delete button_json;
	delete e_nombre;
	delete s_maxjug;
	delete s_puntaje;
	delete spin_x;
	delete spin_y;
	delete spin_nivel;
	delete barraEstado;
}

BarraEstado* BoxOpcionesBasicas::getBarra(){
	return this->barraEstado;
}

void BoxOpcionesBasicas::cargar(const std::string& fileName){
	Json::Value mapa;
	levantarMapa(fileName,mapa);	
	setNombre(mapa);
	setPuntaje(mapa);
	setMaxJug(mapa);
	setNivel(mapa);
	setX(mapa);
	setY(mapa);
	levantarTablero(mapa);
}

void BoxOpcionesBasicas::levantarMapa(const std::string& fileName, Json::Value& mapa){
	std::string auxStr = fileName;
	std::ifstream ifs;
	ifs.open(auxStr.c_str());
	if (ifs.is_open()){
		std::stringstream ss;
		Json::Reader reader;
		std::string str((std::istreambuf_iterator<char>(ifs)), 
				std::istreambuf_iterator<char>());
		reader.parse(str, mapa);
	ifs.close();
	}
}

void BoxOpcionesBasicas::setNombre(const Json::Value& mapa){
	Json::Value::Members keys = mapa.getMemberNames();
	this->e_nombre->set_text(keys[0]);
}

void BoxOpcionesBasicas::setPuntaje(Json::Value& mapa){
	this->s_puntaje->set_value(mapa[this->e_nombre->get_text()]["puntaje_para_ganar"].asDouble());
}

void BoxOpcionesBasicas::setMaxJug(Json::Value& mapa){
	this->s_maxjug->set_value(mapa[this->e_nombre->get_text()]["max_jugadores"].asDouble());
}

void BoxOpcionesBasicas::setNivel(Json::Value& mapa){
	this->spin_nivel->set_value(mapa[this->e_nombre->get_text()]["nivel"].asDouble());
}

void BoxOpcionesBasicas::setX(Json::Value& mapa){
	this->spin_x->set_value(mapa[this->e_nombre->get_text()]["DIM"]["X"].asDouble());
	this->tablero->setX((int)spin_x->get_value());
}

void BoxOpcionesBasicas::setY(Json::Value& mapa){
	this->spin_y->set_value(mapa[this->e_nombre->get_text()]["DIM"]["Y"].asDouble());
	this->tablero->setY((int)spin_y->get_value());
}

void BoxOpcionesBasicas::levantarTablero(Json::Value& mapa){
	tablero->deserializar(mapa);
}

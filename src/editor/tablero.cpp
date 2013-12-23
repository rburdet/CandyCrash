#include "tablero.h"

#include <gtkmm.h>
#include <typeinfo>
#include <string>

#define MAX_COLS 14
#define MIN_COLS 7
#define MIN_FILAS 7
#define MAX_FILAS 14
#define SIZE 40
#define OFFSET 20
#define HUECODIR "../share/candycrash/imagenes/hueco.png"
#define DEFAULTPATH "../share/candycrash/Mapas/"
#define DEFAULTIMGPATH "../share/candycrash/imagenes/"

#define MAXELEMENTOS 16

Tablero::Tablero(Glib::RefPtr<Gtk::Builder>& builder){
	celdaInteres=NULL;
	builder->get_widget("f_tablero",tablero);
	builder->get_widget("event_tablero",eventos_tablero);
	this->eventos_tablero->set_events(Gdk::BUTTON_PRESS_MASK);
	cantFilas=MIN_FILAS;
	cantColumnas=MIN_COLS;
	agregarFilas(cantFilas);
	agregarColumnas(cantColumnas);
	ultFilClick=MAX_FILAS;
	ultColClick=MAX_COLS;
	celdaInteres=NULL;
	colInteres=NULL;
	actualizarMatriz(cantFilas,cantColumnas);
	this->eventos_tablero->signal_button_press_event().connect(sigc::mem_fun
			(*this,&Tablero::on_click_tablero));
	butonsCambiados.resize(MAXELEMENTOS-1);
}

Tablero::~Tablero(){
	this->borrarTodo();
	for ( unsigned int i = 0 ; i < matrizCeldas.size() ; i++ ){
		for ( unsigned int j = 0 ; j < matrizCeldas[0].size() ; j++ ){
			delete matrizCeldas[i][j];
		}
	}
}

void Tablero::on_cordx_changed(Gtk::SpinButton* spin_x){
	//El +1 aca es xq se dibuja un pedazo afuera
	//el tama;o real es sin ese +1 
	int X = spin_x->get_value_as_int() + 1;
	if 	( X > cantFilas ){
		cantFilas=X;
		agregarFilas(X);
		alargarColumnas(X);
	}else if ( X < cantFilas ){
		borrarSeps(cantFilas,FILAS);
	}
	cantFilas=X;
	actualizarMatriz(cantFilas-1,cantColumnas);
}

void Tablero::on_cordy_changed(Gtk::SpinButton* spin_y){
	int Y = spin_y->get_value_as_int() + 1;
	if ( Y > cantColumnas ) {
		cantColumnas=Y;
		agregarColumnas(Y);
		alargarFilas(Y);
	}else if ( Y < cantColumnas ) {
		borrarSeps(cantColumnas,COLUMNAS);
	}
	cantColumnas=Y;
	actualizarMatriz(cantFilas,cantColumnas-1);
}

void Tablero::alargarFilas(int Y){
	for (int i = 0 ; i < cantFilas ; i++){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*Y,SIZE);
		this->tablero->put(*sep_horizontal,0,i*SIZE);
		this->tablero->show_all();
	}
}

void Tablero::alargarColumnas(int X){
	for ( int i = 0 ; i < cantColumnas ; i++ ){
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*X);
		this->tablero->put(*sep_vertical,i*SIZE,0);
		this->tablero->show_all();
	}
}

void Tablero::agregarFilas(int X){
	borrarSeps(X,FILAS);
	for ( int i= 0 ; i < X ; i++ ){
		Gtk::HSeparator* sep_horizontal = new Gtk::HSeparator();
		sep_horizontal->set_size_request(SIZE*cantColumnas,SIZE);
		this->tablero->put(*sep_horizontal,0,i*SIZE);
	} 
}

void Tablero::agregarColumnas(int Y){
	columnas.clear();
	borrarSeps(Y,COLUMNAS);
	for ( int i = 0 ; i < Y ; i++ ){
		Gtk::VSeparator* sep_vertical = new Gtk::VSeparator();
		sep_vertical->set_size_request(SIZE,SIZE*cantFilas);
		this->tablero->put(*sep_vertical,i*SIZE,0);
		Gtk::Button * button = new Gtk::Button();
		Columna* col = new Columna(Y, button);
		if ( i == Y-1 ){
			this->tablero->show_all();
			break;
		}
		this->columnas.push_back(col);
		columnas[i]->get_boton()->signal_clicked().connect(bind(sigc::mem_fun
					(*this,&Tablero::on_click_boton_tablero),i));
		this->tablero->put(*button,(i+1)*SIZE,0);
	} 
}

//TODO: BORRA T0D0, ver si se puede mejorar para que solo borre lo que agrego
void Tablero::borrarSeps(int cantidadBorrar,int filaOColumna){
	int i = 0;
	std::string type;
	if (filaOColumna == COLUMNAS){
		Gtk::VSeparator* sep = new Gtk::VSeparator(); 
		type = typeid(*sep).name();
		delete sep;
	}else if (filaOColumna == FILAS){
		Gtk::HSeparator* sep = new Gtk::HSeparator(); 
		type = typeid(*sep).name();
		delete sep;
	}

	//std::cout << " un VSeparator es: " << typeid(*vsep).name() << std::endl;
	Glib::ListHandle<Gtk::Widget*> separadores = this->tablero->get_children();
	Glib::ListHandle<Gtk::Widget*>::iterator it = separadores.begin();
	for (; it != separadores.end() ; it++) {
		if ((typeid(**it).name()) == type){
			i++;
			if (i>cantidadBorrar){
				this->tablero->remove(*(*it));
				break;
			}
		}
	}
}

void Tablero::borrarTodo(){
	Glib::ListHandle<Gtk::Widget*> separadores = this->tablero->get_children();
	Glib::ListHandle<Gtk::Widget*>::iterator it = separadores.begin();
	for ( ; it != separadores.end() ; it++) {
		this->tablero->remove(*(*it));
	}
}
//XXX: OJO QUE X E Y DEL GET_POINTER  ESTAN INTERCAMBIADOS, X REFIERE A 
//"longitud", ancho, e Y a "latitud" o largo. un (8,2) para gtk es para mi (2,8)
bool Tablero::on_click_tablero(GdkEventButton* event){
	int x,y;
	int fila,columna;
	this->tablero->get_pointer(x,y); 
	fila = (y-OFFSET)/SIZE;
	columna = (x-OFFSET)/SIZE;
	if (ultFilClick==fila && ultColClick==columna){
		return false;
		//No hago nada
	}else{
		celdaInteres = new Celda(fila,columna);
		this->matrizCeldas[fila][columna]=celdaInteres;
		m_signal_uncheck.emit();
	}
	ultFilClick=fila;
	ultColClick=columna;
	cambiarButons();
	ponerPunto(x,y);
	point->show();
	return true;
}

void Tablero::ponerPunto(int x, int y){
	int fila = (y-OFFSET)/SIZE;
	int columna = (x-OFFSET)/SIZE;
	int centered_x = columna * SIZE+OFFSET-2+SIZE/2;
	int centered_y = fila * SIZE+OFFSET-2+SIZE/2;
	this->tablero->move(*point,centered_x,centered_y);
}

void Tablero::actualizarMatriz(int x, int y){
	matrizCeldas.resize(x);
	for ( int i = 0 ; i < x ; i++ ) {
		matrizCeldas[i].resize(y);
	}
	for ( int i = 0 ; i < x ; i++ ){
		for ( int j = 0 ; j < y ; j++ ){
			matrizCeldas[i][j] = new Celda(i,j);
		}
	}
	crearMatrizBotones(x,y);
	point = new Gtk::Image("../share/candycrash/imagenes/dot.png");

	this->tablero->put(*point,100000,100000);
}

void Tablero::crearMatrizBotones(int x,int y){
	matrizButons.resize(x);
	for ( int i = 0 ; i < x ; i++ ){
		matrizButons[i].resize(y);
		for ( int j = 0 ; j < y ; j++ ) {
			matrizButons[i][j].resize(MAXELEMENTOS-1);
		}
	}
}

void Tablero::on_adj_changed_tablero(Gtk::SpinButton* spinbutton,int id){
	if (!celdaInteres)
		return;
	this->celdaInteres->on_adj_changed(spinbutton, id);
	matrizButons[celdaInteres->getX()][celdaInteres->getY()][id]=
		spinbutton->get_value();
	butonsCambiados[id]=spinbutton;
	//butonsCambiados.push_back(spinbutton);
}

//void Tablero::getValoresButons(){
//	for (int i=0 ; i < MAXELEMENTOS ; i++){
//		matrizButons[celdaInteres->getX()][celdaInteres->getY()][i];
//	}
//}

void Tablero::cambiarButons(){
	for ( unsigned int i = 0 ; i < butonsCambiados.size() ; i++ ){
		//butonsCambiados[i]->set_value(0.00);
		if (matrizButons[celdaInteres->getX()][celdaInteres->getY()][i]){
			butonsCambiados[i]->set_value
				(matrizButons[celdaInteres->getX()][celdaInteres->getY()][i]);
		}else if (butonsCambiados[i]){
			butonsCambiados[i]->set_value(0);
		}
	}
	//butonsCambiados.clear();
}

void Tablero::on_click_boton_tablero(int id){
	if (colInteres != columnas[id]){
		colInteres = columnas[id];
		cambiarButonsColumnas();
	}
}

void Tablero::cambiarButonsColumnas(){
	for ( unsigned int i = 0 ; i < butonsColumnasCambiados.size() ; i++ ){
		butonsColumnasCambiados[i]->set_value(0.00);
	}
	butonsColumnasCambiados.clear();
}

void Tablero::on_adjCols_changed_tablero(Gtk::SpinButton* spinbutton, int id){
	if (!colInteres)
		return;
	this->colInteres->on_adj_changed(spinbutton,id);	
	butonsColumnasCambiados.push_back(spinbutton);
}

void Tablero::jsonColumnas(Json::Value& nivel,const std::string& nombre){
	std::stringstream ss;
	for ( unsigned i = 0 ; i < columnas.size() ; i++ ) {
		Json::Value aux(Json::arrayValue);
		for ( int j = 0 ; j < 16 ; j++ ){
			 aux.append(columnas[i]->getInfo()->getProb_piezas(j));
		}
		ss<<i;
		nivel[nombre]["columnas"][ss.str()] = aux;
		ss.str("");
	}
	nivel[nombre]["fondo"]=this->imagenFondo;
}

void Tablero::jsonCeldas(Json::Value& nivel,const std::string& nombre){
	std::stringstream streamFila;
	std::stringstream streamColumna;
	for ( unsigned  int i = 0 ; i < matrizCeldas.size() ; i++ ) {
		streamFila << i;
		for ( unsigned int j = 0 ; j < matrizCeldas[0].size() ; j++ ){
			streamColumna << j;
			if ( matrizCeldas[i][j]->isHueco() ){
				nivel[nombre]["celdas"][streamColumna.str()][streamFila.str()]
					["probabilidades"] = -1;
			}else{
				Json::Value aux(Json::arrayValue);
				for ( int k = 0 ; k < 16 ; k ++ ){
					aux.append(matrizCeldas[i][j]->getInfo()->
							getProb_piezas(k));
				}
				nivel[nombre]["celdas"][streamColumna.str()][streamFila.str()]
					["probabilidades"] = aux;
			}
			nivel[nombre]["celdas"][streamColumna.str()][streamFila.str()]
				["fondo"] = matrizCeldas[i][j]->getImage();
			streamColumna.str("");
		}
		streamFila.str("");
	}
}


void Tablero::on_image_changed_tablero(Gtk::FileChooser* fileChooser){
	std::string path;
	unsigned int aux; 
	aux = fileChooser->get_filename().find_last_of("/\\");
	path=fileChooser->get_filename().substr(aux+1);
	path = std::string(DEFAULTIMGPATH)+ path;
	this->celdaInteres->setImage(path);
	Gtk::Image* img = new Gtk::Image(fileChooser->get_filename());
	img->set_size_request(SIZE,SIZE);
	this->tablero->put(*img,this->celdaInteres->getY()*SIZE+OFFSET,
			this->celdaInteres->getX()*SIZE+OFFSET);
	this->tablero->show_all();
}


void Tablero::on_image_fondo_changed_tablero(Gtk::FileChooser* fileChooser){
	std::string path;
	unsigned int aux; 
	aux = fileChooser->get_filename().find_last_of("/\\");
	path=fileChooser->get_filename().substr(aux+1);
	path = std::string(DEFAULTIMGPATH) +"fondos/" + path;
	this->imagenFondo = path;
}

void Tablero::on_check_button_tablero(){
	if (this->celdaInteres)
		this->celdaInteres->setHueco();
	putHueco(this->celdaInteres->getY()*SIZE+OFFSET,this->celdaInteres->getX()*SIZE+OFFSET);
}

void Tablero::putHueco(int x , int y){
	Gtk::Image* img = new Gtk::Image(HUECODIR);
	img->set_size_request(SIZE,SIZE);
	this->tablero->put(*img,x,y);
	img->show();
}

void Tablero::setX(int X){
	this->cantFilas = X;
}

void Tablero::setY(int Y){
	this->cantColumnas = Y;
}

void Tablero::deserializar(Json::Value& mapa){
	actualizarMatriz(cantFilas, cantColumnas);
	Json::Value::Members keys = mapa.getMemberNames();
	std::string nombre= keys[0];
	Json::Value celdas = mapa[nombre]["celdas"];
	Json::Value columnas = mapa[nombre]["columnas"];
	deserializarCeldas(celdas);
	deserializarColumnas(columnas);
	this->imagenFondo = mapa[nombre]["fondo"].asString();
	std::cout << " voy a levantar mapa 5" << std::endl;
}

void Tablero::deserializarCeldas(Json::Value& celdas){
	std::stringstream streamFila;
	std::stringstream streamColumna;
	for ( int i = 0 ; i < matrizCeldas.size() ; i++ ){
		streamFila << i;
		for ( int j = 0 ; j < matrizCeldas[0].size() ; j++ ){
			std::string imgPath;
			Info* auxInfo = new Info();
			streamColumna << j;
			if ( (imgPath=celdas[streamColumna.str()][streamFila.str()]["fondo"].asString()) != ""){
				matrizCeldas[i][j]->setImage(imgPath);
			}
			if (!celdas[streamColumna.str()][streamFila.str()]["probabilidades"].isArray()){
				matrizCeldas[i][j]->setHueco();
				putHueco(j*SIZE+OFFSET,i*SIZE+OFFSET);
			}else{
				for ( int k=0 ; k < 16 ; k++ ){
					auxInfo->setProb_piezas(celdas[streamColumna.str()][streamFila.str()]["probabilidades"][k].asInt(),k);
				}
				matrizCeldas[i][j]->setInfo(auxInfo);
			}
			streamColumna.str("");
		}
		streamFila.str("");
	}
}

void Tablero::deserializarColumnas(Json::Value& columnas){
 std::stringstream ss;
 for ( int i = 0 ; i < matrizCeldas.size() ; i++ ){
	Info* auxInfo = new Info();
	ss << i ;
	for ( int j=0 ; j < 16 ; j++ ) {
		auxInfo->setProb_piezas(columnas[ss.str()][j].asInt(),j);
	}
	if (auxInfo)
		this->columnas[i]->setInfo(auxInfo);
	ss.str("");
 }
}

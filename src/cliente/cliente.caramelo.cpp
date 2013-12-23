#include "cliente.caramelo.h"
#include <string>

#define DEFAULTIMAGEDIR "../Imagenes/"
#define SIZE 40

Caramelo::Caramelo(int idCaramelo, const std::string& imgDir,int i, int j) :
	image(imgDir) {
	this->moviendo = false;
	this->x = i;
	this->y = j;
	this->x_pos = i * SIZE + 20;
	this->y_pos = j * SIZE + 20;
	this->idCaramelo = idCaramelo;
	this->set_relief(Gtk::RELIEF_NONE);
	this->image.set_size_request(SIZE,SIZE);
	//this->set_image(image);
	this->add(image);
	this->show();
	this->signal_button_press_event().connect(
		sigc::mem_fun(
			this,
			&Caramelo::onButtonPressEvent
		)
	);
}

int Caramelo::getX(){
	return x;
}

int Caramelo::getXPos(){
	return x_pos;
}

int Caramelo::getY(){
	return y;
}

int Caramelo::getYPos(){
	return y_pos;
}

void Caramelo::setX(int x){
	this->x = x;
}

void Caramelo::setXPos(int x){
	this->x_pos = x;
}

void Caramelo::setY(int y){
	this->y = y;
}

void Caramelo::setYPos(int y){
	this->y_pos = y;
}

bool Caramelo::mover(int x, int y){
	if(this->x_mov == x && this->y_mov == y)
		return false;

	return this->mover();
}
bool Caramelo::mover(){
	return this->moviendo;
}

bool Caramelo::setMoviendo(bool moviendo, int x, int y){
	this->setMoviendo(moviendo);
	this->x_mov = x;
	this->y_mov = y;
}
bool Caramelo::setMoviendo(bool moviendo){
	this->moviendo = moviendo;
}

void Caramelo::opacar(){
	double aux = this->get_opacity() - 0.1;
	set_opacity(aux);
}

void Caramelo::hacerAparecer(){
	double aux = this->get_opacity() + 0.1;
	set_opacity(aux);
}

bool Caramelo::visible(){
	return (this->get_opacity()!=0);
}

bool Caramelo::fullyVisible(){
	return (this->get_opacity()==1);
}

bool Caramelo::onButtonPressEvent(GdkEventButton *event){
	this->m_signal_clicked.emit();
	return true;
}

Caramelo::type_signal_clicked Caramelo::signal_clicked(){
	return this->m_signal_clicked;
}

void Caramelo::set_relief(Gtk::ReliefStyle newstyle){

	Glib::RefPtr<Gtk::StyleContext> stylecontext =
		this->get_style_context();

	switch(newstyle){
		case Gtk::RELIEF_NORMAL:
			stylecontext->add_class("event_box_relief_normal");
			break;
		default:
			stylecontext->remove_class("event_box_relief_normal");
			break;
	}

	stylecontext->context_save();
	this->queue_draw();
};

void Caramelo::raiseMe(){
	this->get_window()->raise();
}

#include "cliente.ipwindow.h"
#include <iostream>
#include <string>

using std::string;

Ipwindow::Ipwindow()
	: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_Button_conectar("Conectar"), /*m_check("Active para registrarse"), */
	m_Button_registrar("Registrar"),
	img("../share/candycrash/imagenes/star_big.png") {
	set_size_request(350, 700);
	set_title("Conectate");

	m_VBox.pack_start(menubar, Gtk::PACK_SHRINK, 0);

	menubar.signal_quit().connect(sigc::mem_fun(*this, &Ipwindow::on_salir));
	menubar.signal_help().connect(sigc::mem_fun(*this,&Ipwindow::on_help));
	menubar.signal_about().connect(sigc::mem_fun(*this,&Window::on_about));

	//m_Button_conectar.override_color(Gdk::RGBA("cornsilk"),Gtk::STATE_FLAG_NORMAL);
	add(m_VBox);

	m_VBox.pack_start(img, true, false, 0);

	m_VBox.pack_start(m_host, true, false, 0);
	m_host.set_label(string("Ip:Puerto"));

	m_host.set_text(string("localhost:9000"));

	m_VBox.pack_start(m_user, true, false, 0);
	m_user.set_label(string("Usuario"));

	m_pass.set_visibility(false);
	m_VBox.pack_start(m_pass, true, false, 0);
	m_pass.set_label(string("Password"));

	//m_VBox.pack_start(m_check, true, false, 0);

	m_VBox.pack_start(m_text, true, true, 0);

	m_VBox.add(m_HBox);

	but_box.pack_start(m_Button_conectar, true, true, 80);
	but_box.pack_start(m_Button_registrar, true, true, 80);
	m_Button_box.pack_start(but_box, true, true, 80);
	m_Button_conectar.signal_clicked().connect(sigc::mem_fun(*this,
				&Ipwindow::on_button_conectar));

	m_Button_registrar.signal_clicked().connect(sigc::mem_fun(*this,
				&Ipwindow::on_button_registrar));
	m_VBox.pack_start(m_Button_box, true, false, 0);
	m_Button_conectar.set_can_default();
	m_Button_conectar.grab_default();

	Glib::RefPtr<Gtk::StyleContext> stylecontext =
		m_Button_conectar.get_style_context();
	stylecontext->add_class("btn");
	stylecontext->context_save();

	stylecontext =
		m_Button_registrar.get_style_context();
	stylecontext->add_class("btn1");
	stylecontext->context_save();

	show_all_children();
	show_all();
}

Ipwindow::~Ipwindow(){}

void Ipwindow::on_button_conectar(){
	string ip = m_host.get_text();
	string user = m_user.get_text();
	string pass = m_pass.get_text();
	//bool active = m_check.get_active();
	this->set_editable(false);
	//m_signal_conectar.emit(ip, user, pass, active);
	m_signal_conectar.emit(ip, user, pass, false);
}

void Ipwindow::on_button_registrar(){
	string ip = m_host.get_text();
	string user = m_user.get_text();
	string pass = m_pass.get_text();
	//bool active = m_check.get_active();
	this->set_editable(false);
	//m_signal_conectar.emit(ip, user, pass, active);
	m_signal_conectar.emit(ip, user, pass, true);
}

Ipwindow::type_signal_conectar Ipwindow::signal_conectar(){
	return m_signal_conectar;
}

void Ipwindow::set_editable(bool is_editable){
	this->m_host.set_editable(is_editable);
	this->m_user.set_editable(is_editable);
	this->m_pass.set_editable(is_editable);

	this->m_Button_conectar.set_sensitive(is_editable);
}

void Ipwindow::set_text(std::string& str){
	this->m_text.set_text(str);
}

void Ipwindow::mensaje(Json::Value& data){
	string str = data["msj"].asString();
	this->set_editable(true);
	this->set_text(str);
}

bool Ipwindow::onClose(){
	//TODO: tildes
	if(this->dialog("Desea Cerrar?", "No podra seguir jugando")){
		this->hide();
		Gtk::Main::quit();
	}

	return true;
}

void Ipwindow::on_salir(){
	this->onClose();
}

void Ipwindow::on_help(){
	Gtk::MessageDialog dialog(*this, "Ayuda");
	dialog.set_secondary_text("Para conectarse Ud tener tener una cuenta.\n"
			"Si ya esta registrado asegurese de que el boton "
			"\"Active para registrarse\" este desactivado. Luego coloque el "
			"ip:puerto al que quiera conectarse en el casillero \"Ip:Puerto\","
			"por ejemplo 127.0.0.1:9000 .\n"
			"Luego coloque su nombre de usuario y su contrasena en los campos"
			" \"Usuario\" y \"Password\" respectivamente. \n"
			"Luego presione \"Conectar\" \n"
			"\n\n\n"
			"Si ud no tiene una cuenta, active el boton \"Active para regisrarse\""
			"ingrese el nombre de usuario deseado y luego la contrasena que "
			"usara para conectarse cada vez que entre al juego en los campos"
			"\"Usuario\" y \"Password\" respectivamente.\n"
			"Luego, desactivelo y procesa a conectarse con el usuario recientemente"
			"creado\n\n");
	dialog.run();
}

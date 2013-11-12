#include "cliente.ipwindow.h"
#include <iostream>

using std::string;

Ipwindow::Ipwindow()
	: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_Button_conectar("Conectar"), m_check("Registrarse?") {
	set_size_request(200, 100);
	set_title("Conectate");

	add(m_VBox);

	m_host.set_max_length(50);
	m_host.select_region(0, m_host.get_text_length());
	m_VBox.pack_start(m_host);

	m_host.set_text(string("localhost:9000"));

	m_user.set_max_length(50);
	m_user.select_region(0, m_user.get_text_length());
	m_VBox.pack_start(m_user);

	m_pass.set_max_length(50);
	m_pass.select_region(0, m_pass.get_text_length());
	m_pass.set_visibility(false);
	m_VBox.pack_start(m_pass);

	m_VBox.pack_start(m_check);

	m_VBox.pack_start(m_text);

	// Note that add() can also be used instead of pack_xxx()
	m_VBox.add(m_HBox);

	m_Button_conectar.signal_clicked().connect( sigc::mem_fun(*this,
				&Ipwindow::on_button_conectar) );
	m_VBox.pack_start(m_Button_conectar);
	m_Button_conectar.set_can_default();
	m_Button_conectar.grab_default();

	show_all_children();
	show_all();
}

Ipwindow::~Ipwindow(){}

void Ipwindow::on_button_conectar(){
	string ip = m_host.get_text();
	string user = m_user.get_text();
	string pass = m_pass.get_text();
	bool active = m_check.get_active();
	this->set_editable(false);
	m_signal_conectar.emit(ip, user, pass, active);
}

Ipwindow::type_signal_conectar Ipwindow::signal_conectar(){
	return m_signal_conectar;
}

void Ipwindow::set_editable(bool is_editable){
	this->m_host.set_editable(is_editable);
	this->m_user.set_editable(is_editable);
	this->m_pass.set_editable(is_editable);

	if(is_editable)
		this->m_Button_conectar.show();
	else
		this->m_Button_conectar.hide();

}

void Ipwindow::set_text(std::string& str){
	this->m_text.set_text(str);
}

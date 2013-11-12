#include "cliente.ipwindow.h"
#include <iostream>

using std::string;

Ipwindow::Ipwindow()
	: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_Button_conectar("Conectar") {
	set_size_request(200, 100);
	set_title("Gtk::Entry");

	add(m_VBox);

	m_host.set_max_length(50);
	m_host.select_region(0, m_host.get_text_length());
	m_VBox.pack_start(m_host);

	m_user.set_max_length(50);
	m_user.select_region(0, m_user.get_text_length());
	m_VBox.pack_start(m_user);

	m_pass.set_max_length(50);
	m_pass.select_region(0, m_pass.get_text_length());
	m_VBox.pack_start(m_pass);

	m_VBox.pack_start(m_check);

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

	m_signal_conectar.emit(ip, user, pass, active);
}

Ipwindow::type_signal_conectar Ipwindow::signal_conectar(){
	return m_signal_conectar;
}

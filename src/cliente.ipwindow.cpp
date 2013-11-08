#include "cliente.ipwindow.h"
#include <iostream>

using std::string;
Ipwindow::Ipwindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Button_conectar("Conectar")
{
  set_size_request(200, 100);
  set_title("Gtk::Entry");

  add(m_VBox);

  m_Entry.set_max_length(50);
  m_Entry.select_region(0, m_Entry.get_text_length());
  m_VBox.pack_start(m_Entry);

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

void Ipwindow::on_button_conectar()
{
	std::cout<< m_Entry.get_text()<<std::endl;
	string ip = m_Entry.get_text();
	m_signal_conectar.emit(string(ip));
}

Ipwindow::type_signal_conectar Ipwindow::signal_conectar(){
	return m_signal_conectar;
}

#include "cliente.main_window.h"
#include <string>

using std::string;

MainWindow::MainWindow()
	: m_VBox(Gtk::ORIENTATION_VERTICAL),
	m_button_send("_Send", true)
{
	set_title("Gtk::TextView example");
	set_border_width(5);
	set_default_size(400, 200);

	add(m_VBox);

	//Add the TreeView, inside a ScrolledWindow, with the button underneath:
	m_ScrolledWindow1.add(m_TextView1);

	//Only show the scrollbars when they are necessary:
	m_ScrolledWindow1.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_VBox.pack_start(m_ScrolledWindow1);

	m_ScrolledWindow2.add(m_TextView2);
	m_ScrolledWindow2.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_VBox.pack_start(m_ScrolledWindow2);

	//Add buttons:
	m_VBox.pack_start(m_button_send, Gtk::PACK_SHRINK);

	//Connect signals:
	m_button_send.signal_clicked().connect( sigc::mem_fun(*this,
				&MainWindow::on_mensaje) );

	fill_buffers();
	m_TextView1.set_buffer(m_refTextBuffer1);
	m_TextView2.set_buffer(m_refTextBuffer2);
	show_all_children();
	show_all();
}

void MainWindow::fill_buffers(){
	m_refTextBuffer1 = Gtk::TextBuffer::create();
	m_refTextBuffer1->set_text("Buffer 1");

	m_refTextBuffer2 = Gtk::TextBuffer::create();
	m_refTextBuffer2->set_text("Buffer 2");

}

MainWindow::~MainWindow(){
}

void MainWindow::on_mensaje(){
	string data = m_refTextBuffer2->get_text();
	m_signal_mensaje.emit(data);
}

MainWindow::type_signal_mensaje MainWindow::signal_mensaje(){
	return m_signal_mensaje;
}
void MainWindow::setText(std::string& str){
	m_refTextBuffer1->set_text(str);
}


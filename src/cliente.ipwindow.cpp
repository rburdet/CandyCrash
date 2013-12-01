#include "cliente.ipwindow.h"
#include <iostream>

using std::string;

Ipwindow::Ipwindow()
	: m_VBox(Gtk::ORIENTATION_VERTICAL),
	menubar(Gtk::manage(new Gtk::MenuBar())),
	m_Button_conectar("Conectar"), m_check("Active para registrarse"), img("../imagenes/star_big.png") {
	set_size_request(350, 700);
	set_title("Conectate");

	m_VBox.pack_start(*menubar, Gtk::PACK_SHRINK, 0);

	Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
	Gtk::MenuItem *menuitem_help = Gtk::manage(new Gtk::MenuItem("_Help", true));
	menubar->append(*menuitem_file);
	menubar->append(*menuitem_help);

	Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
	Gtk::Menu *helpmenu = Gtk::manage(new Gtk::Menu());

	menuitem_file->set_submenu(*filemenu);
	menuitem_help->set_submenu(*helpmenu);

	Gtk::MenuItem *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
	Gtk::MenuItem *sub_menuitem_help = Gtk::manage(new Gtk::MenuItem("_Help", true));
	Gtk::MenuItem *menuitem_about = Gtk::manage(new Gtk::MenuItem("_About", true));
	menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &Ipwindow::on_salir));
	filemenu->append(*menuitem_quit);
	helpmenu->append(*sub_menuitem_help);
	helpmenu->append(*menuitem_about);

	this->set_background_image(string("../imagenes/fondos/stripes.jpg"));
	//override_background_color(Gdk::RGBA("crimson"),Gtk::STATE_FLAG_NORMAL);
	m_Button_conectar.override_color(Gdk::RGBA("cornsilk"),Gtk::STATE_FLAG_NORMAL);
	add(m_VBox);

	m_VBox.pack_start(img, true, false, 0);

	//m_host.set_max_length(50);
	//m_host.select_region(0, m_host.get_text_length());
	m_VBox.pack_start(m_host, true, false, 0);
	m_host.set_label(string("Host"));

	m_host.set_text(string("localhost:9000"));

	//m_user.set_max_length(50);
	//m_user.select_region(0, m_user.get_text_length());
	m_VBox.pack_start(m_user, true, false, 0);
	m_user.set_label(string("Usuario"));

	//m_pass.set_max_length(50);
	//m_pass.select_region(0, m_pass.get_text_length());
	m_pass.set_visibility(false);
	m_VBox.pack_start(m_pass, true, false, 0);
	m_pass.set_label(string("Password"));

	m_VBox.pack_start(m_check, true, false, 0);

	m_VBox.pack_start(m_text, true, true, 0);

	// Note that add() can also be used instead of pack_xxx()
	m_VBox.add(m_HBox);

	m_Button_box.pack_start(m_Button_conectar, true, true, 80);
	m_Button_conectar.signal_clicked().connect( sigc::mem_fun(*this,
				&Ipwindow::on_button_conectar) );
	m_VBox.pack_start(m_Button_box, true, false, 0);
	m_Button_conectar.set_can_default();
	m_Button_conectar.grab_default();

	Glib::RefPtr<Gtk::CssProvider> cssprov = Gtk::CssProvider::create();
	cssprov->load_from_path("../imagenes/style.css");
	Glib::RefPtr<Gtk::StyleContext> stylecontext = m_Button_conectar.get_style_context();
	stylecontext->add_provider(cssprov, GTK_STYLE_PROVIDER_PRIORITY_USER);
	stylecontext->add_class("btn");
	stylecontext->context_save();

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

void Ipwindow::on_salir(){
	Gtk::MessageDialog dialog(*this,"Desea cerrar?",false,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
	dialog.set_secondary_text("Asegurese de generar antes de salir");
	int opc=dialog.run();
	if (opc==Gtk::RESPONSE_OK){
		this->hide();
		Gtk::Main::quit();
	}
}

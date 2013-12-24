#include "cliente.nivel_siguiente.h"
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

using std::string;

NivSigWindow::~NivSigWindow() {}
NivSigWindow::NivSigWindow() :
	m_text("Has ganado. Elige que mapa quieres jugar:"),
	m_Button_crear("Jugar"),
	m_Button_salir("Salir"),
	m_VBox() {

	set_size_request(350, 350);
	set_title("Ganaste!");

	add(m_HBox);
	m_HBox.pack_start(m_VBox, true, true, 5);
	m_VBox.pack_start(m_text, true, false, 0);
	m_VBox.pack_start(m_ScrolledMapas, true, true, 0);
	m_VBox.pack_start(but_box, false, false, 10);
	m_ScrolledMapas.add(partidas);
	m_ScrolledMapas.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

	but_box.pack_start(m_Button_crear, true, true, 10);
	but_box.pack_start(m_Button_salir, true, true, 10);

	m_Button_crear.signal_clicked().connect(sigc::mem_fun(*this,
		&NivSigWindow::on_button_crear));
	m_Button_salir.signal_clicked().connect(sigc::mem_fun(*this,
		&NivSigWindow::on_button_salir));

	Glib::RefPtr<Gtk::StyleContext> stylecontext = 
		this->partidas.get_style_context();
	stylecontext->add_class("mapas_list");
	stylecontext->context_save();

	stylecontext = m_Button_crear.get_style_context();
	stylecontext->add_class("btn");
	stylecontext->context_save();

	stylecontext = m_Button_salir.get_style_context();
	stylecontext->add_class("btn1");
	stylecontext->context_save();

	stylecontext = m_ScrolledMapas.get_style_context();
	stylecontext->add_class("scrolled-tree");
	stylecontext->context_save();

	this->show_all();
}

void NivSigWindow::mensaje(Json::Value& data){
	if(data["event"].asInt() != EVENT_GET_MAPS)
		return;

	this->partidas.clearRows();
	if(data["mapas"].isObject()){
		Json::Value mapas = data["mapas"];
		Json::Value::Members nombres = mapas.getMemberNames();
		string key;
		int value;
		for(int i =0; i < nombres.size(); i++){
			key = nombres[i];
			value = mapas[key].asInt();
			this->partidas.addRow(value, key);
		}
	}
}

NivSigWindow::type_signal_salir NivSigWindow::signal_salir(){
	return this->m_signal_salir;
}
NivSigWindow::type_signal_crear NivSigWindow::signal_crear(){
	return this->m_signal_crear;
}

void NivSigWindow::on_button_crear(){
	std::string name;
	int nivel;
	this->partidas.getSelected(nivel, name);

	if(nivel == 0)
		return;

	this->m_signal_crear.emit(name, nivel);
}

void NivSigWindow::on_button_salir(){
	this->m_signal_salir.emit();
}

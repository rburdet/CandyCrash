#include "menupersistidor.h"

#include <string>

#define DEFAULTDIR "../share/candycrash/Mapas/"

MenuPersistidor::MenuPersistidor(BoxOpcionesBasicas* aPersistir, Glib::RefPtr<Gtk::Builder>& builder){
	this->settings=aPersistir;
	builder->get_widget("Nuevo",itemNuevo);
	builder->get_widget("Abrir",itemAbrir);
	builder->get_widget("Guardar",itemGuardar);
	//builder->get_widget("Guardar_Como",itemGuardar_Como);
	itemNuevo->signal_activate().connect(sigc::mem_fun(*this,&MenuPersistidor::Nuevo));
	itemAbrir->signal_activate().connect(sigc::mem_fun(*this,&MenuPersistidor::Abrir));

	itemGuardar->signal_activate().connect(sigc::mem_fun(*this,&MenuPersistidor::Guardar));
	//itemGuardar_Como->signal_activate().connect(sigc::mem_fun(*this,&MenuPersistidor::Guardar_Como));
	
}

void MenuPersistidor::Guardar(){
	if (settings->getNombre() == ""){
		//TODO: Llamar a Save As
	}else{
		settings->getBarra()->mostrar("Se ha guardado " +std::string(DEFAULTDIR)+ settings->getNombre());	
	}
	this->settings->generar();
}


void MenuPersistidor::Abrir(){
	Gtk::FileChooserDialog dialog("Abrir",Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	Gtk::FileFilter filter_text;
	filter_text.set_name("Archivos .map");
	filter_text.add_pattern("*.map");
	dialog.add_filter(filter_text);
	int result = dialog.run();
	switch (result){
		case(Gtk::RESPONSE_OK):{
			std::string fileName = dialog.get_filename();
			this->settings->cargar(std::string(DEFAULTDIR)+fileName);
			settings->getBarra()->mostrar("Se ha cargado " +std::string(DEFAULTDIR)+ fileName);	
			break;
		}
		case(Gtk::RESPONSE_CANCEL):{
			break;
		}
		default:{
			break;
		}
	}
}

//void MenuPersistidor::Guardar_Como(){
//	Gtk::FileChooserDialog dialog("Guardar Como",Gtk::FILE_CHOOSER_ACTION_SAVE);
//	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
//	dialog.add_button("Select", Gtk::RESPONSE_OK);
//	dialog.run();
//	this->settings->generar();
//}


void MenuPersistidor::Nuevo(){}


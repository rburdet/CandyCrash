#include "menupersistidor.h"

#include <string>

#define DEFAULTDIR "../share/candycrash/Mapas/"

MenuPersistidor::MenuPersistidor(BoxOpcionesBasicas* aPersistir, Glib::RefPtr<Gtk::Builder>& builder){
	this->settings=aPersistir;
	builder->get_widget("Abrir",itemAbrir);
	builder->get_widget("Guardar",itemGuardar);
	itemAbrir->signal_activate().connect(sigc::mem_fun(*this,&MenuPersistidor::Abrir));

	itemGuardar->signal_activate().connect(sigc::mem_fun(*this,&MenuPersistidor::Guardar));
	
}

void MenuPersistidor::Guardar(){
	if (settings->getNombre() == ""){
	}else{
		settings->getBarra()->mostrar("Se ha guardado " +std::string(DEFAULTDIR)+ settings->getNombre());	
	}
	this->settings->generar();
}


void MenuPersistidor::Abrir(){
	Gtk::FileChooserDialog dialog("Abrir",Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_current_folder(DEFAULTDIR);
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
			this->settings->cargar(fileName);
			settings->getBarra()->mostrar(std::string(DEFAULTDIR)+ fileName);	
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

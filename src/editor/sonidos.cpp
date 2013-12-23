#include "sonidos.h"

#define DEFAULTDIR "../share/candycrash/sounds/"
Sonidos::Sonidos(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("buttonrep1",buttonrep1);
	builder->get_widget("buttonrep2",buttonrep2);

	builder->get_widget("filechooserbutton1",fileChooser1);
	builder->get_widget("filechooserbutton2",fileChooser2);

	fileChooser1->set_current_folder(DEFAULTDIR);
	fileChooser2->set_current_folder(DEFAULTDIR);

	buttonrep1->signal_clicked().connect(sigc::bind(sigc::mem_fun
			(*this, &Sonidos::reproducir),fileChooser1));
	buttonrep2->signal_clicked().connect(sigc::bind(sigc::mem_fun
			(*this, &Sonidos::reproducir),fileChooser2));
}
		
void Sonidos::jsonSonidos(Json::Value& nivel,const std::string& nombre){
	std::string path1="";
	std::string path2="";
	unsigned int aux; 
	if (fileChooser1->get_filename() != ""){
		aux = fileChooser1->get_filename().find_last_of("/\\");
		path1=fileChooser1->get_filename().substr(aux+1);
		path1 = std::string(DEFAULTDIR) + path1;
	}
	nivel[nombre]["sonidos"]["mover"]=std::string(path1);
	
	if (fileChooser2->get_filename() != ""){
		aux = fileChooser2->get_filename().find_last_of("/\\");
		path2=fileChooser2->get_filename().substr(aux+1);
		path2 = std::string(DEFAULTDIR) + path2;
	}
	nivel[nombre]["sonidos"]["destruir"]=std::string(path2);
	
	
}

void Sonidos::reproducir(Gtk::FileChooser* fileChooser){
	SoundPlayer::play(fileChooser->get_filename());
}


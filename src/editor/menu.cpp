#include "menu.h"
#include <string>
#include <iostream>

Menu::Menu(Glib::RefPtr<Gtk::Builder>& builder){
	builder->get_widget("menubar1",menubar);
	builder->get_widget("help",help);
	builder->get_widget("about",about);
	help->signal_activate().connect(sigc::mem_fun(*this,&Menu::displayHelp));
	about->signal_activate().connect(sigc::mem_fun(*this,&Menu::displayAbout));
}

void Menu::displayHelp(){
	window = new Gtk::Window();
	Gtk::MessageDialog dialog(*window, "Ayuda");
	std::string aux = 
		"Para crear un nivel satisfactoriamente Ud deberia seguir"
		" los siguientes pasos:\n"
		"\t 1. Insertar un nombre en el campo \"Nombre\". Tenga en cuenta que"
		" el nombre del archivo generado y el nombre del mapa son iguales. \n"
		"\t 2. Seleccionar un puntaje maximo para ganar el nivel. \n"
		"\t 3. Seleccionar la cantidad maxima de jugadores que pueden jugar "
		"ese nivel en la casilla \"Max Jug\".\n"
		"\t 4. Seleccionar el nivel del mapa a jugar en la casilla \"Nivel\".\n"
		"\t 5. Seleccionar la cantidad de filas ( X ) y columnas ( Y ).\n"
		"\n\n"
		"Para elegir las probabilidades Ud puede hacerlo mediante columnas o"
		" seleccionando cada celda en particular y eligiendo valores para "
		"cualquiera de los 16 caramelos, la probabilidad insertada para cada"
		" celda solo sera para una configuracion inicial dado que el regimen de"
		" aparicion durante el juego sera a traves de la probabilidad indicada "
		"para cada columna. "
		"\n "
		"Ud puede elegir una imagen de fondo para el tablero, bajo la pestana "
		"\"Columnas\" en el selector de archivos indicado por \"Fondo\" ."
		"Tambien puede elegir una imagen de fondo para cada celda, o si quiere"
		", un hueco para esa celda. Para eso tiene que presionar bajo la "
		"pestana \"Celdas\" el selector de archivo o el boton \"Hueco\" "
		"respectivamente."
		"\n\n\n\n\n"
		"Aclaracion:\n"
		"\t Las probabilidades se computaran como numeros y no como "
		"porcentajes. Por ejemplo teniendo 13 para el boton rojo, 13 para un"
		"bar horizontal verde y 13 para un bar vertical amarillo significa "
		"que ambos van a tener la misma probabilidad de aparecer.";
	dialog.set_secondary_text(aux);
	dialog.run();
	delete window;
}

void Menu::displayAbout(){
	window = new Gtk::Window();
	Gtk::MessageDialog dialog(*window, "Editor de niveles");
	std::string aux = "Version 1.0\n\n\n\n"
		" Autores: \n"
		" \t Rodrigo Burdet \n"
		" \t Nicolas Cisco "; 
	dialog.set_secondary_text(aux);
	dialog.run();
	delete window;
}

Menu::~Menu(){
	delete window;
	delete menubar;
	delete about;
	delete help;
}

//#include "common.creadortablero.h"
#include <iostream>
#include <jsoncpp/json/json.h>
#include "server.listador.h"
#include "common.creadortablero.h"
#include "cliente.tablerojuego.h"
#include <gdk-pixbuf/gdk-pixbuf.h>


int main(int argc , char* argv[]){
	Gtk::Main kit(argc,argv);
	Gtk::Window window;
	window.set_size_request(500,500);
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file("../imagenes/fondo.jpg",50,50);
	Gtk::Image imagen(pixbuf);
	imagen.set_size_request(100,500);
	imagen.set_hexpand(true);
	imagen.show();
	window.add(imagen);

	//CreadorTablero mapa("prueba2.map");
	//std::cout << mapa.getTablero() << std::endl;
	//TableroJuego tablero(mapa.getTablero());
	kit.run(window);
	return 0;
}

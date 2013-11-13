//#include "common.creadortablero.h"
#include <iostream>
#include <jsoncpp/json/json.h>
#include "server.listador.h"
#include "common.creadortablero.h"
#include "cliente.tablerojuego.h"

int main(int argc , char* argv[]){
	Gtk::Main kit(argc,argv);

	CreadorTablero mapa("posta.map");
	//std::cout << mapa.getTablero();
	TableroJuego tablero(mapa.getTablero());
	kit.run();
	return 0;
}

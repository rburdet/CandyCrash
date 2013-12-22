#ifndef COLUMNA_H
#define COLUMNA_H

#include <vector>
#include <gtkmm.h>
#include "info.h"

/** Entidad que modela las columnas para la aparicion de caramelos.
 */
class Columna{
	public:
		/** Crea la columna x, que tenga el botonColumna activado.
		 * @param x: Numero de columna
		 * @param botonColumna: Boton para activar la columna
		 */
		Columna(int x, Gtk::Button* botonColumna);
		~Columna();
		/** Cambia la probabilidad de la columna. 
		 */
		void on_adj_changed(Gtk::SpinButton* spinbutton,int id);
		Gtk::Button* get_boton();
		Info* getInfo();
		void setInfo(Info* nuevaInfo);
	private:
		Gtk::Button* boton;
		int pos;
		Info* probs;
};

#endif 

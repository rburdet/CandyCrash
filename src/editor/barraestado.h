#ifndef BARRAESTADO_H
#define BARRAESTADO_H

#include <gtkmm.h>
#include <string>

/** Estatus bar para indicar mensajes de generacion de mapas.
 */
class BarraEstado{
	public:
		/**Constructor del status bar usado para mostrar mensajes
		 * @param builder[in]: Encargado de levantar el widget desde el archivo
		 * de glade.
		 */
		explicit BarraEstado(Glib::RefPtr<Gtk::Builder>& builder);
		~BarraEstado();

		/**Senal para mostrar
		 */
		void onMostrar(Gtk::Entry* entryArchivo);

		/**Muestra textoAMostrar en la barra de estados.
		 */
		void mostrar(const std::string& textoAMostrar);

	private:
		Gtk::Statusbar* barraEstado;
};

#endif

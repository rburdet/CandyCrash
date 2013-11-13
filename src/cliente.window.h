#ifndef __CLIENTE_WINDOW_H__
#define __CLIENTE_WINDOW_H__
#include <jsoncpp/json/json.h>
#include <gtkmm.h>

/** Interfaz de ventana para todas las ventanas del cliente.
 */
class Window : public Gtk::Window {
	public:
		/** Metodo que reciben los mensajes del servidor, que deben mostrar.
		 */
		virtual void mensaje(Json::Value& data) = 0;

		/** Signal para avisarle al cliente que tienen que mandar un mensaje
		 */
		typedef sigc::signal<void, Json::Value> type_signal_mensaje;
		type_signal_mensaje signal_mensaje(){
			return m_signal_mensaje;
		}

	protected:
		type_signal_mensaje m_signal_mensaje;

};

#endif

#ifndef CLIENT_CLIENTE_INTERFACE_H
#define CLIENT_CLIENTE_INTERFACE_H
#include <jsoncpp/json/json.h>

/** Interfaz de cliente.
 * Es usada por ThreadListener para evitar la referencia circular con cliente.
 */
class ClienteInterface {
	public:
		/** Agrega un nuevo mensaje a la cola de mensajes (bloque el mutex de la cola)
		 */
		virtual void nuevoMensaje(Json::Value& msj) = 0;
};
#endif

#ifndef CLIENT_CLIENTE_INTERFACE_H
#define CLIENT_CLIENTE_INTERFACE_H
#include <jsoncpp/json/json.h>

class ClienteInterface {
	public:
		virtual void nuevoMensaje(Json::Value& msj) = 0;
};
#endif

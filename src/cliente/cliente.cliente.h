#ifndef CLIENT_CLIENTE_H
#define CLIENT_CLIENTE_H

#include <gtkmm.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <queue>
#include "cliente.socket_connect.h"
#include "cliente.ipwindow.h"
#include "cliente.cliente_interface.h"
#include "cliente.thread_listener.h"
#include "../common/common.mutex.h"
#include "cliente.window.h"

/** Clase cliente. Viene a ser la entidad que representa al programa.
 */
class Cliente : public ClienteInterface {
	public: 
		Cliente();
		~Cliente();
		/** Muestra la ventana de Ip y login.
		 */
		void mostrarVentanaIP();

	private:
		//Informacion del usuario conectado
		Json::Value userData; 
		//Ventana actual que se esta mostrando
		Window* ventanaActual; 
		//Thread que atiende las lecturas y las carga en la cola de mensajes
		ThreadListener *listener; 		
		// Cola de mensajes (todo lo entrante se recibe por aca)
		std::queue<Json::Value> mensajes;
		//Mutex de la cola
		Mutex mensajesMutex;
		//Timeout (de gtk) para leer la cola
		sigc::connection connTimeout; 
		/** Funcion que se encarga de leer la cola.
		 * Es ejecutada en el main del gtk como un "timeout", si se devuelve false, 
		 * se deja de llamar a la funcion.
		 */
		bool onTimeout();

		/** Metodo que se llama al recibir un evento de login desde el servidor
		 */
		void onLogin(int code, Json::Value& data);

		/** Metodo que se llama al recibir un evento de logout desde el servidor
		 */
		void onLogout(int code, Json::Value& data);

		/** Metodo que se llama al recibir un evento de crear o unirse a partida 
		 * (desde el servidor).
		 */
		void onGame(int code, Json::Value& data);

		/** Callback que se ejecua cuando la ventana de IP da la se~nal de que se puede conectar
		 */
		void conectar(std::string ip, std::string user, std::string pass, bool check);

		/** Manda un msje a traves del socket
		 */
		void sendMsj(Json::Value data);

		/** Metodo para pushear un nuevo mensaje a la cola de mensajes (bloquea mutex)
		 */
		virtual void nuevoMensaje(Json::Value& msj);
};

#endif

#ifndef __COMMON_SOCKET_IO_H__
#define __COMMON_SOCKET_IO_H__
#include <string>
#include <jsoncpp/json/json.h>
#include "common.socket.h"

/** Socket de entrada y salida.
 * Implementa metodos de escribir y leer con firma.
 */
class SocketIO : public virtual Socket {
	public:
		SocketIO();
		explicit SocketIO(unsigned int fd);
		~SocketIO();

		/** read: lee mensaje.
		 * Lee mensaje de socket. Primero lee prefijo de longitud, dependiendo de eso, lee el mensaje, y despues lee la firma, la validacion de la misma es opcional, pero siempre se la leera. El metodo se encarga de la transformacion de la data json entrante en objeto.
		 * En el caso de haber una firma invalida, se saldra con error.
		 * @param [out] data informacion proveniente del socket.
		 * @param [in] key llave usada para la comprobacion de la firma.
		 * @param [in] check opcional, (true por defecto) si es verdadero se chequeara la firma.
		 * @return 0 ok, -1 para cualqier error de lectura de socket, 1 para error de validacion de firma, 2 para problemas de parseo de json.
		 */
		int read(Json::Value &data, const std::string& key, bool check=true);

		/** write: envia el mensaje.
		 * Agrega prefijo de longitud (en bytes) y firma del mensaje. El prefijo de longitud es un uint32_t que esta puesto con hton, despues viene el mensaje (en json) y por ultimo la firma del mensaje @see hmac_msje().
		 * El largo se refiere solo al largo del mensaje, no incluye el largo de la firma que siempre sera el mismo. El orden es <prefijo_longitud><mensaje><firma>.
		 * El metodo se encarga de serializar.
		 * @param [in] data data a enviar
		 * @param [in] key llave para la firma
		 * @return codigo de error, 0 ok, resto error.
		 */
		int write(const Json::Value &data, const std::string& key);
};

#endif

#ifndef __COMMON_HMAC_H__
#define __COMMON_HMAC_H__
#include <string>

#define HMAC_LENGTH 16

/** hmac_msje: genera firma hmac de un mensaje.
 * @param [in] key clave privada
 * @param [in] msje mensaje
 * @param [out] output firma del mensaje utilizando la clave.
 */
void hmac_msje(const std::string &key, const std::string &msje, std::string 
		&output);


#endif

#ifndef __COMMON_MD5_H__
#define __COMMON_MD5_H__
#define MD5_BYTE_LENGTH 16
#include <string>

/** md5_digest: Algoritmo de reduccion criptografico.
 *
 * @param [in] initial_msg mensaje a hacerle el md5
 * @param [out] digest md5 del mensaje
 */
void md5_digest(const std::string & initial_msg, std::string &digest);
#endif


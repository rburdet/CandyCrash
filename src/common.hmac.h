#ifndef __COMMON_HMAC_H__
#define __COMMON_HMAC_H__
#include <string>

#define HMAC_LENGTH 32

void hmac_msje(const std::string &key, const std::string &msje, std::string &output );


#endif

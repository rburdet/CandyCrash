#ifndef __COMMON_MD5_H__
#define __COMMON_MD5_H__
#define MD5_BYTE_LENGTH 16
#include <string>

void md5_digest(const std::string & initial_msg, std::string &digest);
#endif


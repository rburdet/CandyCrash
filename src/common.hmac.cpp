#include "common.hmac.h"
#include "common.md5.h"

// Tiene que ser 64 para md5
#define BLOCKSIZE 64

using std::string;

void hmac_msje(const string &key, const string &msje, string &output){
	string my_key = key;
	string o_key_pad;
	string i_key_pad;

	if(my_key.size() > BLOCKSIZE){
		md5_digest(key, my_key);
	}
	if(my_key.size() < BLOCKSIZE){
		int max = BLOCKSIZE - my_key.size();
		for(int i=0; i < max; i++){
			my_key.push_back('0');
		}
	}

	int size = my_key.size();

	for(int i=0; i < size; i++){
		o_key_pad.push_back(my_key[i] ^ 0x5C);
		i_key_pad.push_back(my_key[i] ^ 0x36);
	}

	string md5_ipad;
	md5_digest(i_key_pad+msje, md5_ipad);

	output = "";

	md5_digest(o_key_pad+md5_ipad, output);
}

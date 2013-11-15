#ifndef STAR_H
#define STAR_H

#include "common.caramelo.h"

class Star : public Caramelo{
	public:
		Star(int idCaramelo,const std::string& imgDir) : Caramelo(idCaramelo,imgDir){}
		~Star(){}
		bool mover(){return false;}
};


#endif

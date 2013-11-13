#ifndef STAR_H
#define STAR_H

#include "common.caramelo.h"

class Star : public Caramelo{
	public:
		Star(int idCaramelo) : Caramelo(idCaramelo){}
		~Star(){}
		bool mover(){return false;}
};


#endif

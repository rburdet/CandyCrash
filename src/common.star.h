#ifndef STAR_H
#define STAR_H

#include "common.caramelo.h"

class Star : public Caramelo{
	public:
		Star(int idCaramelo,const std::string& imgDir,int i , int j) : Caramelo(idCaramelo,imgDir,i,j){}
		~Star(){}
		bool mover(){return false;}
};


#endif

#ifndef STAR_H
#define STAR_H

#include "cliente.caramelo.h"
#include <string>

class Star : public Caramelo{
	public:
		Star(int idCaramelo,const std::string& imgDir,int i , int j) :
			Caramelo(idCaramelo,imgDir,i,j){}
		~Star(){}
		bool mover(){return false;}
};


#endif

#ifndef BAR_H
#define BAR_H

#include "common.caramelo.h"

class Bar : public Caramelo{
	public:
		Bar(int idCaramelo,bool acostado,const std::string& imageName,int i, int j) : Caramelo(idCaramelo,imageName,i,j){
			this->acostado = acostado;
		}
		~Bar(){}
		bool mover(){return false;}
		
	private:
		bool acostado;
};

#endif


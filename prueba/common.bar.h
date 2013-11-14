#ifndef BAR_H
#define BAR_H

#include "common.caramelo.h"

class Bar : public Caramelo{
	public:
		Bar(int idCaramelo,bool acostado,const std::string& imageName) : Caramelo(idCaramelo,imageName){
			this->acostado = acostado;
		}
		~Bar(){}
		bool mover(){return false;}
		
	private:
		bool acostado;
};

#endif


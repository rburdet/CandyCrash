#ifndef BAR_H
#define BAR_H

#include "common.caramelo.h"

class Bar : public Caramelo{
	public:
		Bar(int idCaramelo,bool acostado) : Caramelo(idCaramelo){
			this->acostado = acostado;
		}
		~Bar(){}
		bool mover(){return false;}
		
	private:
		bool acostado;
};

#endif


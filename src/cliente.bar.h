#ifndef BAR_H
#define BAR_H

#include "cliente.caramelo.h"

class Bar : public Caramelo{
	public:
		Bar(int idCaramelo,bool acostado,const std::string& imageName,int i, int j) : Caramelo(idCaramelo,imageName,i,j){
			this->acostado = acostado;
		}
		~Bar(){}
		
	private:
		bool acostado;
};

#endif


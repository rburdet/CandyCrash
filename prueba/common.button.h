#ifndef BUTTON_H
#define BUTTON_H

#include "common.caramelo.h"

class Button : public Caramelo {
	public:
		Button(int idCaramelo,const std::string& imageName) : Caramelo(idCaramelo,imageName){}
		~Button(){}
		bool mover(){return false;}
};

#endif

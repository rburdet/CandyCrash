#ifndef CARAMELO_H
#define CARAMELO_H

#include <string>
#include <gtkmm.h>

class Candy : public Gtk::Image{
	public:
		Candy(std::string nombreCaramelo) : Gtk::Image(nombreCaramelo){}
		~Candy(){}
	
};


#endif 

#ifndef FACTORY_CARAMELOS_H
#define FACTORY_CARAMELOS_H

#include <string>
#include "common.button.h"
#include "common.bar.h"
#include "common.star.h"
#include "common.caramelos.h"

#define DEFAULTIMAGEDIR "../imagenes/"
#define SIZE 50
class CandyFactory{
	public:
		static Caramelo* crearCaramelo(int id){
			Caramelo* car = NULL;
			std::string IMAGEDIR(DEFAULTIMAGEDIR);
			switch (id){
				case HUECO:
					break;
				case RED_BUTTON:
					car = new Button(id,IMAGEDIR+"button_red.png");
					break;
				case BLUE_BUTTON:
					car = new Button(id,IMAGEDIR+"button_blue.png");
					break;
				case GREEN_BUTTON:
					car = new Button(id,IMAGEDIR+"button_green.png");
					break;
				case PURPLE_BUTTON:
					car = new Button(id,IMAGEDIR+"button_purple.png");
					break;
				case YELLOW_BUTTON:
					car = new Button(id,IMAGEDIR+"button_yellow.png");
					break;
				case RED_HORBAR:
					car = new Bar(id,true,IMAGEDIR+IMAGEDIR+"bar_hor_red.png");
					break;
				case BLUE_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_blue.png");
					break;
				case GREEN_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_green.png");
					break;
				case PURPLE_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_purple.png");
					break;
				case YELLOW_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_yellow.png");
					break;
				case RED_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_red.png");
					break;
				case BLUE_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_blue.png");
					break;
				case GREEN_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_green.png");
					break;
				case PURPLE_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_purple.png");
					break;
				case YELLOW_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_yellow.png");
					break;
				case STAR:
					car = new Star(id,IMAGEDIR+"star.png");
					break;
			}
			return car;
		}
};
#endif

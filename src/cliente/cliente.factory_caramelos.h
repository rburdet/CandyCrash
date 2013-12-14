#ifndef FACTORY_CARAMELOS_H
#define FACTORY_CARAMELOS_H

#include <string>
#include "cliente.button.h"
#include "cliente.bar.h"
#include "cliente.star.h"
#include "../common/common.caramelos.h"

#define DEFAULTIMAGEDIR "../share/candycrash/imagenes/"
#define SIZE 50
class CandyFactory{
	public:
		static Caramelo* crearCaramelo(int id,int i,int j){
			Caramelo* car = NULL;
			std::string IMAGEDIR(DEFAULTIMAGEDIR);
			switch (id){
				case HUECO:
					break;
				case RED_BUTTON:
					car = new Button(id,IMAGEDIR+"button_red.png",i,j);
					break;
				case BLUE_BUTTON:
					car = new Button(id,IMAGEDIR+"button_blue.png",i,j);
					break;
				case GREEN_BUTTON:
					car = new Button(id,IMAGEDIR+"button_green.png",i,j);
					break;
				case PURPLE_BUTTON:
					car = new Button(id,IMAGEDIR+"button_purple.png",i,j);
					break;
				case YELLOW_BUTTON:
					car = new Button(id,IMAGEDIR+"button_yellow.png",i,j);
					break;
				case RED_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_red.png",i,j);
					break;
				case BLUE_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_blue.png",i,j);
					break;
				case GREEN_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_green.png",i,j);
					break;
				case PURPLE_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_purple.png",i,j);
					break;
				case YELLOW_HORBAR:
					car = new Bar(id,true,IMAGEDIR+"bar_hor_yellow.png",i,j);
					break;
				case RED_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_red.png",i,j);
					break;
				case BLUE_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_blue.png",i,j);
					break;
				case GREEN_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_green.png",i,j);
					break;
				case PURPLE_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_purple.png",i,j);
					break;
				case YELLOW_VERBAR:
					car = new Bar(id,false,IMAGEDIR+"bar_vert_yellow.png",i,j);
					break;
				case STAR:
					car = new Star(id,IMAGEDIR+"star.png",i,j);
					break;
			}
			return car;
		}
};
#endif

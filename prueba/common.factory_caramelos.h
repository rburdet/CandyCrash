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
					car = new Button(id);
					car->set(IMAGEDIR+"button_red.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case BLUE_BUTTON:
					car = new Button(id);
					car->set(IMAGEDIR+"button_blue.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case GREEN_BUTTON:
					car = new Button(id);
					car->set(IMAGEDIR+"button_green.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case PURPLE_BUTTON:
					car = new Button(id);
					car->set(IMAGEDIR+"button_purple.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case YELLOW_BUTTON:
					car = new Button(id);
					car->set(IMAGEDIR+"button_yellow.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case RED_HORBAR:
					car = new Button(id);
					car->set(IMAGEDIR+"bar_hor_red.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case BLUE_HORBAR:
					car = new Bar(id,true);
					car->set(IMAGEDIR+"bar_hor_blue.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case GREEN_HORBAR:
					car = new Bar(id,true);
					car->set(IMAGEDIR+"bar_hor_green.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case PURPLE_HORBAR:
					car = new Bar(id,true);
					car->set(IMAGEDIR+"bar_hor_purple.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case YELLOW_HORBAR:
					car = new Bar(id,true);
					car->set(IMAGEDIR+"bar_hor_yellow.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case RED_VERBAR:
					car = new Bar(id,false);
					car->set(IMAGEDIR+"bar_vert_red.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case BLUE_VERBAR:
					car = new Bar(id,false);
					car->set(IMAGEDIR+"bar_vert_blue.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case GREEN_VERBAR:
					car = new Bar(id,false);
					car->set(IMAGEDIR+"bar_vert_green.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case PURPLE_VERBAR:
					car = new Bar(id,false);
					car->set(IMAGEDIR+"bar_vert_purple.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case YELLOW_VERBAR:
					car = new Bar(id,false);
					car->set(IMAGEDIR+"bar_vert_yellow.png");
					car->set_size_request(SIZE,SIZE);
					break;
				case STAR:
					car = new Star(id);
					car->set(IMAGEDIR+"star.png");
					car->set_size_request(SIZE,SIZE);
					break;
			}
			return car;
		}
};
#endif

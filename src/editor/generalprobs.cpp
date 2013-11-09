#include "generalprobs.h"

#define DEFAULTIMAGEDIR "../../imagenes/"
#define WIDGETNAME "image"
#define RED "red"
#define BLUE "blue"
#define YELLOW "yellow"
#define PURPLE "purple"
#define BUTTON "button"
#define BAR "bar"
#define VERT "vert"
#define HOR "hor"

#include <iostream>

Probs::Probs(Glib::RefPtr<Gtk::Builder>& builder, int startNumber , int finalNumber,std::string frameName){
	std::vector<std::string> array;
	std::vector<std::string> dirArray;
	std::string str;
	builder->get_widget(frameName,aFrame);
	//Creo un vector de 16 nombres con imageN ( con N del 1 al 16 o del 17 al 32)
	std::ostringstream ss;
	for (int i=startNumber ; i <= finalNumber ; i++){
		ss << WIDGETNAME << i ; 
		array.push_back(ss.str());
		ss.str(std::string());
	}
	//Botones
	builder->get_widget(array.at(0),red_button);
	builder->get_widget(array.at(1),blue_button);
	builder->get_widget(array.at(2),green_button);
	builder->get_widget(array.at(3),purple_button);
	builder->get_widget(array.at(4),yellow_button);
	//Minibars
	builder->get_widget(array.at(5),red_minibar_hor);
	builder->get_widget(array.at(6),blue_minibar_hor);
	builder->get_widget(array.at(7),green_minibar_hor);
	builder->get_widget(array.at(8),purple_minibar_hor);
	builder->get_widget(array.at(9),yellow_minibar_hor);

	builder->get_widget(array.at(10),red_minibar_ver);
	builder->get_widget(array.at(11),blue_minibar_ver);
	builder->get_widget(array.at(12),green_minibar_ver);
	builder->get_widget(array.at(13),purple_minibar_ver);
	builder->get_widget(array.at(14),yellow_minibar_ver);

	//Star
	builder->get_widget(array.at(15),star);

	std::string IMAGEDIR(DEFAULTIMAGEDIR);

	//Botones
	red_button->set(IMAGEDIR+"button_red.png");
	blue_button->set(IMAGEDIR+"button_blue.png");
	green_button->set(IMAGEDIR+"button_green.png");
	purple_button->set(IMAGEDIR+"button_purple.png");
	yellow_button->set(IMAGEDIR+"button_yellow.png");

	//Minibars
	red_minibar_hor->set(IMAGEDIR+"bar_hor_red.png");
	blue_minibar_hor->set(IMAGEDIR+"bar_hor_blue.png");
	green_minibar_hor->set(IMAGEDIR+"bar_hor_green.png");
	purple_minibar_hor->set(IMAGEDIR+"bar_hor_purple.png");
	yellow_minibar_hor->set(IMAGEDIR+"bar_hor_yellow.png");

	red_minibar_ver->set(IMAGEDIR+"bar_vert_red.png");
	blue_minibar_ver->set(IMAGEDIR+"bar_vert_blue.png");    	
	green_minibar_ver->set(IMAGEDIR+"bar_vert_green.png");
	purple_minibar_ver->set(IMAGEDIR+"bar_vert_purple.png");
	yellow_minibar_ver->set(IMAGEDIR+"bar_vert_yellow.png");

	//Star
	star->set(IMAGEDIR+"star.png");
}

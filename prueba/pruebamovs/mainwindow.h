#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "caramelo.h"

class MainWindow : public Gtk::Window{
	public:
		MainWindow() : carRed("button_red.png"), carBlue("button_blue.png"), boton("boton"){
			this->set_title("movimiento");
			this->set_default_size(500,300);
			this->set_position(Gtk::WIN_POS_CENTER_ALWAYS);
			this->fixed=new Gtk::Fixed();
			this->add(*(this->fixed));
			x=150;
			y=30;
			fixed->put(*(dynamic_cast<Gtk::Image*>(&carRed)),y,30);
			fixed->put(*(dynamic_cast<Gtk::Image*>(&carBlue)),x,30);
			fixed->put(boton,300,300);
			this->boton.signal_clicked().connect(sigc::mem_fun(this,&MainWindow::on_click_boton));

		}
		~MainWindow(){}

	private:
		Candy carRed;
		Candy carBlue;
		Gtk::Fixed* fixed;
		Gtk::Button boton;
		sigc::connection conTimeout; 
		int x;
		int y; 

		void on_click_boton(){
			sigc::slot<bool> my_slot = sigc::mem_fun(*this,&MainWindow::onTimeout);
			this->conTimeout = Glib::signal_timeout().connect(my_slot,10);
		}
		bool onTimeout(){
			if ((x!=30) && (y!=150)){
				this->fixed->move(*(dynamic_cast<Gtk::Image*>(&carBlue)),x--,10);
				this->fixed->move(*(dynamic_cast<Gtk::Image*>(&carRed)),y++,10);
				return true;
			}
			return false;
		}
};



#endif

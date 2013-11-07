#include "probceldas.h"

#define FIRSTIMAGE 17
#define LASTIMAGE 32

ProbCeldas::ProbCeldas(Glib::RefPtr<Gtk::Builder>& builder, Tablero* tablero) : Probs(builder,FIRSTIMAGE,LASTIMAGE,"frameceldas"){

	builder->get_widget("spinbutton21",spinbutton21);
	//builder->get_widget("spinbutton22",spinbutton22);
	//builder->get_widget("spinbutton23",spinbutton23);
	//builder->get_widget("spinbutton24",spinbutton24);
	//builder->get_widget("spinbutton25",spinbutton25);
	//builder->get_widget("spinbutton26",spinbutton26);
	//builder->get_widget("spinbutton27",spinbutton27);
	//builder->get_widget("spinbutton28",spinbutton28);
	//builder->get_widget("spinbutton29",spinbutton29);
	//builder->get_widget("spinbutton30",spinbutton30);
	//builder->get_widget("spinbutton31",spinbutton31);
	//builder->get_widget("spinbutton32",spinbutton32);
	//builder->get_widget("spinbutton33",spinbutton33);
	//builder->get_widget("spinbutton34",spinbutton34);
	//builder->get_widget("spinbutton35",spinbutton35);
	//builder->get_widget("spinbutton36",spinbutton36);
	Glib::RefPtr<Gtk::Adjustment> adjustment21 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment21"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment22 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment22"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment23 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment23"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment24 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment24"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment25 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment25"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment26 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment26"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment27 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment27"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment28 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment28"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment29 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment29"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment30 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment30"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment31 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment31"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment32 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment32"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment33 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment33"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment34 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment34"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment35 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment35"));
	//Glib::RefPtr<Gtk::Adjustment> adjustment36 = Glib::RefPtr<Gtk::Adjustment>::cast_dynamic(builder->get_object("adjustment36"));

	adjustment21->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj21_changed),spinbutton21));
	//adjustment22->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj22_changed),spinbutton22));
	//adjustment23->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj23_changed),spinbutton23));
	//adjustment24->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj24_changed),spinbutton24));
	//adjustment25->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj25_changed),spinbutton25));
	//adjustment26->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj26_changed),spinbutton26));
	//adjustment27->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj27_changed),spinbutton27));
	//adjustment28->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj28_changed),spinbutton28));
	//adjustment29->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj29_changed),spinbutton29));
	//adjustment30->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj30_changed),spinbutton30));
	//adjustment31->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj31_changed),spinbutton31));
	//adjustment32->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj32_changed),spinbutton32));
	//adjustment33->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj33_changed),spinbutton33));
	//adjustment34->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj34_changed),spinbutton34));
	//adjustment35->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj35_changed),spinbutton35));
	//adjustment36->signal_value_changed().connect(sigc::bind(sigc::mem_fun(tablero->celdaInteres,&Celda::on_adj36_changed),spinbutton36));
}

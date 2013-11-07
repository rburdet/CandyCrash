#ifndef PROB_CELDAS_H
#define PROB_CELAS_H

#include <gtkmm.h>
#include "generalprobs.h"
#include "tablero.h"

class ProbCeldas : public Probs{
	public:
		ProbCeldas(Glib::RefPtr<Gtk::Builder>& builder,Tablero* tablero);
	private:
		Gtk::Frame * frameProbCeldas;


		Gtk::SpinButton * spinbutton21;
		Gtk::SpinButton * spinbutton22;
		Gtk::SpinButton * spinbutton23;
		Gtk::SpinButton * spinbutton24;
		Gtk::SpinButton * spinbutton25;
		Gtk::SpinButton * spinbutton26;
		Gtk::SpinButton * spinbutton27;
		Gtk::SpinButton * spinbutton28;
		Gtk::SpinButton * spinbutton29;
		Gtk::SpinButton * spinbutton30;
		Gtk::SpinButton * spinbutton31;
		Gtk::SpinButton * spinbutton32;
		Gtk::SpinButton * spinbutton33;
		Gtk::SpinButton * spinbutton34;
		Gtk::SpinButton * spinbutton35;
		Gtk::SpinButton * spinbutton36;
		Glib::RefPtr<Gtk::Adjustment> adjustment21;
		Glib::RefPtr<Gtk::Adjustment> adjustment22;
		Glib::RefPtr<Gtk::Adjustment> adjustment23;
		Glib::RefPtr<Gtk::Adjustment> adjustment24;
		Glib::RefPtr<Gtk::Adjustment> adjustment25;
		Glib::RefPtr<Gtk::Adjustment> adjustment26;
		Glib::RefPtr<Gtk::Adjustment> adjustment27;
		Glib::RefPtr<Gtk::Adjustment> adjustment28;
		Glib::RefPtr<Gtk::Adjustment> adjustment29;
		Glib::RefPtr<Gtk::Adjustment> adjustment30;
		Glib::RefPtr<Gtk::Adjustment> adjustment31;
		Glib::RefPtr<Gtk::Adjustment> adjustment32;
		Glib::RefPtr<Gtk::Adjustment> adjustment33;
		Glib::RefPtr<Gtk::Adjustment> adjustment34;
		Glib::RefPtr<Gtk::Adjustment> adjustment35;
		Glib::RefPtr<Gtk::Adjustment> adjustment36;
};

#endif


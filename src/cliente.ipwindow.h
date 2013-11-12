#ifndef IP_WINDOW_H
#define IP_WINDOW_H

#include <gtkmm.h>
#include <string>

class Ipwindow : public Gtk::Window{

	public:
		Ipwindow();
		virtual ~Ipwindow();
		typedef sigc::signal<void, std::string, std::string, std::string, bool> type_signal_conectar;
		type_signal_conectar signal_conectar();
		void set_editable(bool is_editable);
		void set_text(std::string& str);

	protected:
		//Signal handlers:
		void on_button_conectar();

		//Child widgets:
		Gtk::Box m_HBox;
		Gtk::Box m_VBox;
		Gtk::Entry m_host;
		Gtk::Entry m_user;
		Gtk::Entry m_pass;
		Gtk::Label m_text;
		Gtk::CheckButton m_check;
		Gtk::Button m_Button_conectar;
		type_signal_conectar m_signal_conectar;
};

#endif 
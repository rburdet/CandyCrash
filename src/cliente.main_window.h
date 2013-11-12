#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <gtkmm.h>
#include <string>

class MainWindow : public Gtk::Window {

	public:
		MainWindow();
		virtual ~MainWindow();
		typedef sigc::signal<void, std::string> type_signal_mensaje;
		type_signal_mensaje signal_mensaje();
		void setText(std::string& str);

	protected:
		//Signal handlers:
		void on_mensaje();

		//Child widgets:
		Gtk::Box m_VBox;
		Gtk::ScrolledWindow m_ScrolledWindow1;
		Gtk::ScrolledWindow m_ScrolledWindow2;
		Gtk::TextView m_TextView1;
		Gtk::TextView m_TextView2;
		Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1, m_refTextBuffer2;
		Gtk::Button m_button_send;
		type_signal_mensaje m_signal_mensaje;

		void fill_buffers();
};

#endif 

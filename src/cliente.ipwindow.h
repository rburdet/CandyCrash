#ifndef IP_WINDOW_H
#define IP_WINDOW_H

#include <gtkmm.h>
#include <string>
#include "cliente.window.h"

#include "widgets/cliente.label_entry.h"
#include "widgets/cliente.menu_bar.h"

/** Ventana de login y seleccionar servidor.
 */
class Ipwindow : public Window {

	public:
		Ipwindow();
		virtual ~Ipwindow();
		/** Signal para que el Cliente realice la coneccion con el servidor
		 */
		typedef sigc::signal<void, std::string, std::string, std::string, bool> type_signal_conectar;
		type_signal_conectar signal_conectar();
		void set_editable(bool is_editable);
		void set_text(std::string& str);
		virtual void mensaje(Json::Value& data);

	protected:
		//Signal handlers:
		void on_button_conectar();

		//Child widgets:
		Gtk::Box m_HBox;
		Gtk::Box m_VBox;
		//Gtk::MenuBar menubar;
		MenuBar menubar;

		Gtk::Image img;

		LabelEntry m_host;
		LabelEntry m_user;
		LabelEntry m_pass;

		Gtk::Label m_text;
		Gtk::CheckButton m_check;
		Gtk::HBox m_Button_box;
		Gtk::Button m_Button_conectar;
		type_signal_conectar m_signal_conectar;

		virtual bool onClose();

	private:
		void on_salir();

};

#endif 

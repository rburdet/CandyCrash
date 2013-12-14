#ifndef __CLIENTE_WINDOW_H__
#define __CLIENTE_WINDOW_H__
#include <jsoncpp/json/json.h>
#include <gtkmm.h>
#include <string>
#include <vector>
#include "../common/common.logger.h"

/** Interfaz de ventana para todas las ventanas del cliente.
 */
class Window : public Gtk::Window {
	public:
		/** Metodo que reciben los mensajes del servidor, que deben mostrar.
		 */
		virtual void mensaje(Json::Value& data) = 0;
		Window(){
			this->signal_delete_event().connect(sigc::mem_fun(this, &Window::_onClose));
			Glib::RefPtr<Gtk::StyleContext> stylecontext = this->get_style_context();
			stylecontext->add_class("user_window");
			stylecontext->context_save();
		}

		/** Signal para avisarle al cliente que tienen que mandar un mensaje
		 */
		typedef sigc::signal<void, Json::Value> type_signal_mensaje;
		type_signal_mensaje signal_mensaje(){
			return m_signal_mensaje;
		}

		void on_about(){
			Gtk::MessageDialog dialog(*this, "Candy Crush");
			std::string aux = PACKAGE_STRING "\n\n\n\n"
				" Autores: \n"
				" \t Rodrigo Burdet \t \n"
				" \t Nicolas Cisco \t "; 
			dialog.set_secondary_text(aux);
			dialog.run();
		}

	protected:
		type_signal_mensaje m_signal_mensaje;

		bool _onClose(GdkEventAny* event){
			return this->onClose();
		}

		virtual bool onClose(){
			return false;
		}

		void set_background_image(std::string path){
			try{
				std::string style_sheet = ".image_bg { background-image: url('"+path+"');}";
				Glib::RefPtr<Gtk::StyleContext> stylecontext = this->get_style_context();
				Glib::RefPtr<Gtk::CssProvider> cssprov = Gtk::CssProvider::create();
				cssprov->load_from_data(style_sheet);
				stylecontext->add_provider(cssprov, GTK_STYLE_PROVIDER_PRIORITY_USER);
				stylecontext->add_class("image_bg");
				stylecontext->context_save();
			} catch(Glib::Error err){
				Logger::log("Error seteando el fondo: '"+path+"' \nError: '"+err.what()+"'");
			}
		}

		bool dialog(const char* pri, const char* sec){
			std::string p(pri);
			std::string s(sec);
			return this->dialog(p, s);
		}

		bool dialog(const std::string& pri, const std::string& sec){
			Gtk::MessageDialog dialog(*this, pri, false, Gtk::MESSAGE_QUESTION,
					Gtk::BUTTONS_OK_CANCEL);
			dialog.set_secondary_text(sec);
			int opc = dialog.run();

			if(opc == Gtk::RESPONSE_OK)
				return true;

			return false;
		}
};

#endif

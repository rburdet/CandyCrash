#ifndef __MENU_BAR_H__
#define __MENU_BAR_H__
#include <gtkmm.h>
#include "../cliente.sound_player.h"

class MenuBar : public Gtk::MenuBar {
	public:
		MenuBar() :
			menuitem_file(Gtk::Stock::FILE),
			menuitem_help(Gtk::Stock::HELP),
			menuitem_pref(Gtk::Stock::PREFERENCES),
			menuitem_mute("Un/_Mute", true),
			menuitem_quit(Gtk::Stock::QUIT),
			sub_menuitem_help(Gtk::Stock::HELP),
			menuitem_about(Gtk::Stock::ABOUT)
		{
			// -- Estilos
			Glib::RefPtr<Gtk::StyleContext> stylecontext = this->get_style_context();
			stylecontext->add_class("menu_bar");
			stylecontext->context_save();
			// ------
			
			this->append(menuitem_file);
			this->append(menuitem_pref);
			this->append(menuitem_help);
			this->menuitem_file.set_submenu(filemenu);
			this->menuitem_pref.set_submenu(prefmenu);
			this->menuitem_help.set_submenu(helpmenu);
			this->filemenu.append(menuitem_quit);
			this->helpmenu.append(sub_menuitem_help);
			this->helpmenu.append(menuitem_about);
			this->prefmenu.append(menuitem_mute);

			this->menuitem_quit.signal_activate().connect(sigc::mem_fun(
						*this, &MenuBar::on_quit));
			this->sub_menuitem_help.signal_activate().connect(sigc::mem_fun(
						*this, &MenuBar::on_help));
			this->menuitem_about.signal_activate().connect(sigc::mem_fun(
						*this, &MenuBar::on_about));
			this->menuitem_mute.signal_activate().connect(sigc::mem_fun(
						*this, &MenuBar::on_mute));
		}

		~MenuBar(){
		}

		typedef sigc::signal<void> type_signal_activate;

		type_signal_activate signal_quit(){
			return this->m_signal_quit;
		}
		type_signal_activate signal_help(){
			return this->m_signal_help;
		}
		type_signal_activate signal_about(){
			return this->m_signal_about;
		}

	protected:
		Gtk::MenuItem menuitem_mute;
		Gtk::Menu filemenu, helpmenu, prefmenu;
		Gtk::ImageMenuItem menuitem_quit, sub_menuitem_help, menuitem_about, menuitem_pref, menuitem_help, menuitem_file;

		type_signal_activate m_signal_quit;
		type_signal_activate m_signal_help;
		type_signal_activate m_signal_about;

		void on_quit(){
			this->m_signal_quit.emit();
		}

		void on_help(){
			this->m_signal_help.emit();
		}

		void on_about(){
			this->m_signal_about.emit();
		}

		void on_mute(){
			SoundPlayer::togglemute();
		}
};

class MenuBarDisconnect : public MenuBar {
	public:
		MenuBarDisconnect() : menuitem_disconnect(Gtk::Stock::DISCONNECT){
			this->filemenu.prepend(menuitem_disconnect);
			this->menuitem_disconnect.signal_activate().connect(sigc::mem_fun(
						*this, &MenuBarDisconnect::on_disconnect));
		}
		~MenuBarDisconnect(){}

		type_signal_activate signal_disconnect(){
			return this->m_signal_disconnect;
		}

	protected:
		Gtk::ImageMenuItem menuitem_disconnect;
		MenuBar::type_signal_activate m_signal_disconnect;

		void on_disconnect(){
			this->m_signal_disconnect.emit();
		}
};

#endif

#ifndef __LABEL_ENTRY_H__
#define __LABEL_ENTRY_H__

#include <gtkmm.h>

class LabelEntry : public Gtk::HBox {
	public:
		LabelEntry(){
			std::string style_sheet = ".entry { border-radius: 4px ;}";
			Glib::RefPtr<Gtk::StyleContext> stylecontext = entry.get_style_context();
			Glib::RefPtr<Gtk::CssProvider> cssprov = Gtk::CssProvider::create();
			cssprov->load_from_data(style_sheet);
			stylecontext->add_provider(cssprov, GTK_STYLE_PROVIDER_PRIORITY_USER);
			stylecontext->add_class("entry");
			stylecontext->context_save();
			this->pack_start(label);
			this->pack_start(entry);
		}

		void set_label(std::string str){
			this->label.set_text(str);
		}

		void set_text(std::string str){
			this->entry.set_text(str);
		}

		void set_editable(bool e){
			this->entry.set_editable(e);
		}

		void set_visibility(bool e){
			this->entry.set_visibility(e);
		}

		Glib::ustring get_text(){
			return this->entry.get_text();
		}
		~LabelEntry(){}
	protected:
		Gtk::Entry entry;
		Gtk::Label label;
};

#endif

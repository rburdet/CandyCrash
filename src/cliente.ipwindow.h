#ifndef IP_WINDOW_H
#define IP_WINDOW_H

#include <gtkmm.h>
#include <string>

class Ipwindow : public Gtk::Window{

public:
  Ipwindow();
  virtual ~Ipwindow();
  typedef sigc::signal<void, std::string> type_signal_conectar;
  type_signal_conectar signal_conectar();

protected:
  //Signal handlers:
  void on_button_conectar();

  //Child widgets:
  Gtk::Box m_HBox;
  Gtk::Box m_VBox;
  Gtk::Entry m_Entry;
  Gtk::Button m_Button_conectar;
  type_signal_conectar m_signal_conectar;
};

#endif 

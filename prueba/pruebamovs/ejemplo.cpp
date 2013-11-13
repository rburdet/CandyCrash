#include <gtkmm.h>
#include "mainwindow.h"

//GtkWidget *button;
//GtkWidget *fixed;
//
//int i=0;
//int x=15;
//int y=15;
//
//
//void button_clicked(GtkWidget *widget, gpointer data)
//{
//	printf("button clicked \n");
//	i=1;
//}
//
//gboolean move(gpointer data){
//	printf("move i %d\n",i);
//	if (i){
//		gtk_fixed_move(GTK_FIXED(fixed), button, x++, y++);
//	}
//	return 1;
//}


int main( int argc, char *argv[]){
	Gtk::Window * window;

	Gtk::Button * button1;
	Gtk::Button * button2;
	Gtk::Main kit(argc,argv);
	MainWindow * mainWindow = new MainWindow();
	mainWindow->show_all();
	kit.run();
	return 0;
}

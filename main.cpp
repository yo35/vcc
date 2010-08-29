
#include <iostream>
#include <gtkmm/main.h>
#include "clockwindow.h"

using namespace std;

int main(int argc, char *argv[]) {
	
		
	Gtk::Main kit(argc, argv);
	ClockWindow* mainUI = new ClockWindow();
	Gtk::Main::run(*mainUI);
	
	return 0;
}

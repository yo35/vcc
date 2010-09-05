
#include <iostream>
#include <gtkmm/main.h>
#include "clockwindow.h"
#include "params.h"

int main(int argc, char *argv[]) {

	std::cout << "Bonjour" << std::endl;

	Gtk::Main kit(argc, argv);
	gp = new Params();
	ClockWindow* mainUI = new ClockWindow();
	Gtk::Main::run(*mainUI);
	delete gp;

	return 0;
}

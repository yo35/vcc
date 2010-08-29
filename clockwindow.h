
#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include "digitaldial.h"

class ClockWindow : public Gtk::Window {

public:
	ClockWindow();

protected:
	bool on_key_press_event(GdkEventKey* event);
	//bool on_button_press_event(GdkEventButton* event);

private:
	void set_no_actif(int new_no_actif);

	Timer       timer[2];
	DigitalDial dial [2];
	int no_actif;
	Gtk::HBox layout;
};

#endif

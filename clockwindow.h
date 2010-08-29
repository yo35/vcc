
#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include "digitaldial.h"

class ClockWindow : public Gtk::Window {

public:
	ClockWindow();

protected:
	bool on_key_press_event(GdkEventKey* event);
	//bool on_button_press_event(GdkEventButton* event);

private:

	// Event handling
	void on_pause_clicked();

	// Change the
	void set_no_actif(int new_no_actif);

	int no_actif;
	Timer       timer[2];
	DigitalDial dial [2];
	Gtk::Button btn_pause;
	Gtk::Button btn_reset;
	Gtk::HBox   dial_layout;
	Gtk::HBox   btns_layout;
	Gtk::VBox   main_layout;
};

#endif

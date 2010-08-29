
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
	void on_reset_clicked();

	// Modifie le cadran actif
	void set_no_actif(int new_no_actif);

	// Remets les timers à zéro
	void reset_timers();

	// Test si l'un des timers est actif
	bool one_timer_is_active() const;

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

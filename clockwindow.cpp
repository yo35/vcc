
#include "clockwindow.h"
#include "hardwarekeycode.h"
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <iostream>

ClockWindow::ClockWindow() : Gtk::Window(),
	btn_reset(Gtk::Stock::NEW), btn_pause(Gtk::Stock::MEDIA_PAUSE)
{
	// Divers
	no_actif = -1;
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);

	// Réglage des horloges
	reset_timers();

	// Gestion des événements
	btn_reset.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_reset_clicked));
	btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_pause_clicked));

	// Géométrie générale
	for(int i=0; i<2; ++i) {
		dial[i].set_can_focus(true);
		dial[i].set_timer(timer[i]);
		dial_layout.pack_start(dial[i]);
	}
	toolbar.append(btn_reset);
	toolbar.append(btn_pause);
	main_layout.pack_start(toolbar, Gtk::PACK_SHRINK);
	main_layout.pack_start(dial_layout);
	main_layout.set_spacing(5);
	add(main_layout);
	show_all_children();
	dial[0].grab_focus();
}

bool ClockWindow::on_key_press_event(GdkEventKey* event)  {
	std::cout << "Button : " << event->hardware_keycode << std::endl;
	switch(event->hardware_keycode) {

		// Les blancs appuient sur la pendule
		case KEY_A:
		case KEY_Z:
		case KEY_E:
		case KEY_R:
		case KEY_T:
		case KEY_Q:
		case KEY_S:
		case KEY_D:
		case KEY_F:
		case KEY_W:
		case KEY_X:
		case KEY_C:
		case KEY_V:
			set_no_actif(1);
			break;

		// Les noirs appuient sur la pendule
		case KEY_I:
		case KEY_O:
		case KEY_P:
		case KEY_K:
		case KEY_L:
		case KEY_M:
			set_no_actif(0);
			break;

		default:
			break;
	}
	return true;
}

void ClockWindow::on_pause_clicked() {
	set_no_actif(-1);
}

void ClockWindow::on_reset_clicked() {

	// On demande confirmation si l'un des timers est actif
	if(one_timer_is_active()) {
		Gtk::MessageDialog dialog(*this, "Do you really want to start a new game ?",
			false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
		dialog.set_title("Stop this game ?");
		int retval = dialog.run();
		if(retval!=Gtk::RESPONSE_YES)
			return;
	}

	// Remise à zéro proprement dite
	reset_timers();
}

void ClockWindow::set_no_actif(int new_no_actif) {
	if(new_no_actif==no_actif)
		return;

	if(one_timer_is_active()) {
		timer[no_actif].set_mode(Timer::PAUSED);
	}
	no_actif = new_no_actif;
	if(one_timer_is_active()) {
		timer[no_actif].set_mode(Timer::DECREMENT);
	}
}

void ClockWindow::reset_timers() {
	for(int i=0; i<2; ++i) {
		timer[i].set_time(3*60*1000);
		//timer[i].set_time(3*1000);
		timer[i].set_mode(Timer::PAUSED);
	}
}

bool ClockWindow::one_timer_is_active() const {
	return no_actif>=0 && no_actif<2;
}

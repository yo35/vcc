
#include "clockwindow.h"
#include "hardwarekeycode.h"
#include <gtkmm/stock.h>
#include <iostream>

ClockWindow::ClockWindow() : Gtk::Window(),
	btn_break("Play/pause"), btn_reset("New game")
{
	for(int i=0; i<2; ++i) {
		timer[i].set_time(3*60*1000);
		//timer[i].set_time(3*1000);
		timer[i].set_mode(Timer::PAUSED);
		dial [i].set_timer(timer[i]);
		dial_layout.pack_start(dial[i]);
	}
	no_actif = -1;
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);
	btns_layout.set_spacing(5);
	main_layout.set_spacing(5);
	btns_layout.pack_start(btn_break);
	btns_layout.pack_start(btn_reset);
	main_layout.pack_start(dial_layout);
	main_layout.pack_start(btns_layout);
	add(main_layout);
	show_all_children();
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

void ClockWindow::set_no_actif(int new_no_actif) {
	if(new_no_actif==no_actif)
		return;

	if(no_actif>=0 && no_actif<2) {
		timer[no_actif].set_mode(Timer::PAUSED);
	}
	no_actif = new_no_actif;
	if(no_actif>=0 && no_actif<2) {
		timer[no_actif].set_mode(Timer::DECREMENT);
	}
}

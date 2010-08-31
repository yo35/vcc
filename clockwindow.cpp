
#include "clockwindow.h"
#include "hardwarekeycode.h"
#include "timecontroldialog.h"
#include "keymap.h"
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <iostream>
#include <cassert>

ClockWindow::ClockWindow() : Gtk::Window(),
	btn_reset(Gtk::Stock::NEW), btn_pause(Gtk::Stock::MEDIA_PAUSE), btn_tctrl("Time control")
{
	// Divers
	time_control.set_mode(TimeControl::BRONSTEIN);
	//time_control.set_mode(TimeControl::FISCHER);
	//time_control.set_mode(TimeControl::HOUR_GLASS);
	//time_control.set_mode(TimeControl::SIMPLE_DELAY);
	time_control.set_main_time(180*1000);
	time_control.set_increment(  3*1000);
	no_actif = -1;
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);

	// Réglage des horloges
	reset_timers();

	// Gestion des événements
	btn_reset.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_reset_clicked));
	btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_pause_clicked));
	btn_tctrl.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_tctrl_clicked));

	// Géométrie générale
	for(int i=0; i<2; ++i) {
		dial[i].set_can_focus(true);
		dial[i].set_timer(timer[i]);
		dial_layout.pack_start(dial[i]);
	}
	toolbar.append(btn_reset);
	toolbar.append(btn_pause);
	toolbar.append(btn_tctrl);
	main_layout.pack_start(toolbar, Gtk::PACK_SHRINK);
	main_layout.pack_start(dial_layout);
	main_layout.set_spacing(5);
	add(main_layout);
	show_all_children();
	dial[0].grab_focus();
}

bool ClockWindow::on_key_press_event(GdkEventKey* event)  {
	//std::cout << "Hardware KC : " << std::hex << event->hardware_keycode
		//<< "\t Keyval : " << event->keyval << std::endl;
	//Keymap::keycode_to_keyvals(event->hardware_keycode);
	Keymap::Keyval kv = Keymap::keycode_to_cannonical_keyval(event->hardware_keycode);
	std::cout << "Kv: " << kv << "; Name:" << Keymap::keyval_to_string(kv) << std::endl;

	//guint key_up = gdk_keyval_to_upper(event->keyval);
	//std::cout << gdk_keyval_name(key_up) << std::endl;
	//std::cout << "Keycode:" << event->hardware_keycode << std::endl;
	//Keymap::keyval_to_keycode(event->keyval);

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
			on_clock_button_clicked(0);
			break;

		// Les noirs appuient sur la pendule
		case KEY_I:
		case KEY_O:
		case KEY_P:
		case KEY_K:
		case KEY_L:
		case KEY_M:
			on_clock_button_clicked(1);
			break;

		default:
			break;
	}
	return true;
}

void ClockWindow::on_pause_clicked() {
	if(one_timer_is_active())
		stop_timer();
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

void ClockWindow::on_tctrl_clicked() {
	TimeControlDialog dialog(*this, time_control);
	int retval = dialog.run();
	if(retval!=Gtk::RESPONSE_OK)
		return;
	time_control = dialog.get_time_control();
	reset_timers();
}

void ClockWindow::on_clock_button_clicked(int no) {
	assert(no>=0 && no<2);
	if(one_timer_is_active()) {
		if(no_actif==no)
			change_timer();
	}
	else {
		start_timer(1-no);
	}
}

void ClockWindow::start_timer(int no) {
	assert(!one_timer_is_active());
	if(time_control.mode()==TimeControl::HOUR_GLASS)
		timer[1-no].set_mode(Timer::INCREMENT);
	timer[no].set_mode(Timer::DECREMENT);
	no_actif = no;
}

void ClockWindow::change_timer() {
	assert(one_timer_is_active());

	// En hour-glass, le timer inactif est en mode incrément
	if(time_control.mode()==TimeControl::HOUR_GLASS)
		timer[no_actif].set_mode(Timer::INCREMENT);

	// Sinon, il est mis en pause
	else {
		timer[no_actif].set_mode(Timer::PAUSED);

		// En mode fischer ou bronstein, on incrémente le compteur à chaque coup,
		// Rq : uniquement si le compteur est positif
		if((
			time_control.mode()==TimeControl::FISCHER ||
			time_control.mode()==TimeControl::BRONSTEIN) &&
			timer[no_actif].get_time() >= 0)
		{
			int new_time = timer[no_actif].get_time() + time_control.increment(no_actif);
			if(time_control.mode()==TimeControl::BRONSTEIN) {
				if(new_time > bronstein_limit[no_actif])
					new_time = bronstein_limit[no_actif];
				else
					bronstein_limit[no_actif] = new_time;
			}
			timer[no_actif].set_time(new_time);
		}
	}

	// Le nouveau timer actif passe en mode décrement
	no_actif = 1-no_actif;
	timer[no_actif].set_mode(Timer::DECREMENT);
}

void ClockWindow::stop_timer() {
	assert(one_timer_is_active());
	timer[0].set_mode(Timer::PAUSED);
	timer[1].set_mode(Timer::PAUSED);
	no_actif = -1;
}

void ClockWindow::reset_timers() {
	for(int i=0; i<2; ++i) {

		// Mise en pause
		timer[i].set_mode(Timer::PAUSED);

		// Calcul du temps initial
		int start_time = time_control.main_time(i);
		if(time_control.mode()==TimeControl::FISCHER || time_control.mode()==TimeControl::BRONSTEIN)
			start_time += time_control.increment(i);

		// Définition des flags
		timer[i].set_time(start_time);
		if(time_control.mode()==TimeControl::BRONSTEIN)
			bronstein_limit[i] = start_time;
	}
}

bool ClockWindow::one_timer_is_active() const {
	return no_actif>=0 && no_actif<2;
}

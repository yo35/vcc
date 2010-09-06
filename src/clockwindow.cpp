
#include "clockwindow.h"
#include "timecontroldialog.h"
#include "keymap.h"
#include "params.h"
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <cassert>

#ifdef OS_IS_WINDOWS
	#include <winkeyhookdll.h>
#endif

ClockWindow::ClockWindow() : Gtk::Window(),
	btn_reset(Gtk::Stock::NEW), btn_pause(Gtk::Stock::MEDIA_PAUSE), btn_tctrl("Time control")
{
	// Divers
	no_actif = -1;
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);

	// Désactivation de la touche windows
	#ifdef OS_IS_WINDOWS
		set_kbd_hook();
	#endif

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

	// Décodage
	Keymap::Keycode code  = event->hardware_keycode;
	bool test_area[2];
	for(int k=0; k<2; ++k) {
		test_area[k] = (gp->key_area[k].find(code) != gp->key_area[k].end());
	}

	// On appelle la fonction qvb
	int verif = 0;
	for(int k=0; k<2; ++k) {
		if(test_area[k]) {
			on_clock_button_clicked(k);
			++verif;
		}
	}
	assert(verif<2);
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
	TimeControlDialog dialog(*this, gp->time_control);
	int retval = dialog.run();
	if(retval!=Gtk::RESPONSE_OK)
		return;
	gp->time_control = dialog.get_time_control();
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
	if(gp->time_control.mode()==TimeControl::HOUR_GLASS)
		timer[1-no].set_mode(Timer::INCREMENT);
	timer[no].set_mode(Timer::DECREMENT);
	no_actif = no;
}

void ClockWindow::change_timer() {
	assert(one_timer_is_active());

	// En hour-glass, le timer inactif est en mode incrément
	if(gp->time_control.mode()==TimeControl::HOUR_GLASS)
		timer[no_actif].set_mode(Timer::INCREMENT);

	// Sinon, il est mis en pause
	else {
		timer[no_actif].set_mode(Timer::PAUSED);

		// En mode fischer ou bronstein, on incrémente le compteur à chaque coup,
		// Rq : uniquement si le compteur est positif
		if((
			gp->time_control.mode()==TimeControl::FISCHER ||
			gp->time_control.mode()==TimeControl::BRONSTEIN) &&
			timer[no_actif].get_time() >= 0)
		{
			int new_time = timer[no_actif].get_time() + gp->time_control.increment(no_actif);
			if(gp->time_control.mode()==TimeControl::BRONSTEIN) {
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
		int start_time = gp->time_control.main_time(i);
		if(gp->time_control.mode()==TimeControl::FISCHER || gp->time_control.mode()==TimeControl::BRONSTEIN)
			start_time += gp->time_control.increment(i);

		// Définition des flags
		timer[i].set_time(start_time);
		if(gp->time_control.mode()==TimeControl::BRONSTEIN)
			bronstein_limit[i] = start_time;
	}
}

bool ClockWindow::one_timer_is_active() const {
	return no_actif>=0 && no_actif<2;
}

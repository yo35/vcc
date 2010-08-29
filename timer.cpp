
#include "timer.h"
#include <glibmm/main.h>

Timer::Timer() {
	mode = PAUSED;
	curr_time = 0;
	reset_counter();
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Timer::on_timeout_elapses), 150);
}

void Timer::set_mode(Mode new_mode) {
	refresh_time();
	mode = new_mode;
	reset_counter();
	refresh_widget();
}

Timer::Mode Timer::get_mode() const {
	return mode;
}

void Timer::set_time(int new_time) {
	curr_time = new_time;
	reset_counter();
	refresh_widget();
}

int Timer::get_time() const {
	refresh_time();
	return curr_time;
}

int Timer::get_raw_time() const {
	return curr_time;
}

void Timer::refresh_time() const {

	// Si on est en pause, curr_time est supposé être à jour
	if(mode==PAUSED)
		return;

	time_t now;
	time(&now);
	int diff = static_cast<int>(difftime(now, start_at) * 1000.0);
	if(mode==INCREMENT)
		curr_time = time_at_begin + diff;
	else
		curr_time = time_at_begin - diff;
}

bool Timer::on_timeout_elapses() {
	refresh_time();
	refresh_widget();
	return true;
}

void Timer::reset_counter() {
	time_at_begin = curr_time;
	time(&start_at);
}

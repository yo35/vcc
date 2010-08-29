
#include "timer.h"
#include <glibmm/main.h>

Timer::Timer() : Glib::Object() {
	m_mode = PAUSED;
	m_time = 0;
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Timer::on_timeout_elapses), 150);
}

sigc::signal<void> Timer::signal_modified() const {
	return m_signal_modified;
}

bool Timer::on_timeout_elapses() {
	if(m_mode!=PAUSED)
		m_signal_modified.emit();
	return true;
}

void Timer::set_mode(Mode new_mode) {
	if(m_mode==new_mode)
		return;

	// Mise en pause
	if(new_mode==PAUSED) {
		m_time = get_time();
		m_mode = PAUSED;
	}

	// Démarrage du timer
	else {
		time(&m_start_at);
		m_mode = new_mode;
	}

	m_signal_modified.emit();
}

Timer::Mode Timer::get_mode() const {
	return m_mode;
}

void Timer::set_time(int new_time) {
	if(m_mode != PAUSED) {
		m_mode = PAUSED;
	}
	m_time = new_time;
	m_signal_modified.emit();
}

int Timer::get_time() const {

	// Si on est en pause, la valeur du timer est stockée dans m_time
	if(m_mode==PAUSED)
		return m_time;

	// Sinon, il faut calculer la valeur courante à partir de la date de lancement
	// du timer (m_start_at) et de la valeur du timer à cet instant (m_time)
	else {
		time_t now;
		time(&now);
		int diff = static_cast<int>(difftime(now, m_start_at) * 1000.0);
		if(m_mode==INCREMENT)
			return m_time + diff;
		else
			return m_time - diff;
	}
}

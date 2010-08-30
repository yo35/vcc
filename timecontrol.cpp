
#include "timecontrol.h"
#include <cassert>

// Constructeur
TimeControl::TimeControl() {
	m_mode = SIMPLE_DELAY;
	for(int k=0; k<2; ++k) {
		m_main_time[k] = 0;
		m_increment[k] = 0;
	}
}

// Accesseurs
TimeControl::Mode TimeControl::mode() const {
	return m_mode;
}

int TimeControl::main_time(int no) const {
	assert(no>=0 && no<2);
	return m_main_time[no];
}

int TimeControl::increment(int no) const {
	assert(m_mode==FISCHER || m_mode==BRONSTEIN);
	assert(no>=0 && no<2);
	return m_increment[no];
}

// Modifieurs
void TimeControl::set_mode(Mode new_mode) {
	m_mode = new_mode;
}

void TimeControl::set_main_time(int new_main_time, int no) {
	if(no>=0 && no<2)
		m_main_time[no] = new_main_time;
	else {
		m_main_time[0] = new_main_time;
		m_main_time[1] = new_main_time;
	}
}

void TimeControl::set_increment(int new_increment, int no) {
	assert(m_mode==FISCHER || m_mode==BRONSTEIN);
	if(no>=0 && no<2)
		m_increment[no] = new_increment;
	else {
		m_increment[0] = new_increment;
		m_increment[1] = new_increment;
	}
}

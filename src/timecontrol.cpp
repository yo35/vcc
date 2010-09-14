/*
 *    This file is part of Virtual Chess Clock, a chess clock software
 *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

bool TimeControl::both_sides_have_same_time() const {
	bool retval = m_main_time[0]==m_main_time[1];
	if(m_mode==FISCHER || m_mode==BRONSTEIN)
		retval = retval && (m_increment[0]==m_increment[1]);
	return retval;
}

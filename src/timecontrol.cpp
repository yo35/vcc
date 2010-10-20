/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                            *
 ******************************************************************************/


#include "timecontrol.h"

// Constantes pour les types de cadences
const TimeControlType SUDDEN_DEATH(0);
const TimeControlType FISCHER     (1);
const TimeControlType BRONSTEIN   (2);
const TimeControlType HOURGLASS   (3);

// Constructeur
TimeControl::TimeControl() {
	m_mode = SUDDEN_DEATH;
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_main_time[*k] = 0;
		m_increment[*k] = 0;
	}
}

// Accesseurs
TimeControlType TimeControl::mode() const {
	return m_mode;
}

int TimeControl::main_time(const Side &side) const {
	return m_main_time[side];
}

int TimeControl::increment(const Side &side) const {
	assert(m_mode==FISCHER || m_mode==BRONSTEIN);
	return m_increment[side];
}

// Modifieurs
void TimeControl::set_mode(const TimeControlType &new_mode) {
	m_mode = new_mode;
}

void TimeControl::set_main_times(int new_main_times) {
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_main_time[*k] = new_main_times;
	}
}

void TimeControl::set_main_time(int new_main_time, const Side &side) {
	m_main_time[side] = new_main_time;
}

void TimeControl::set_increments(int new_increments) {
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_increment[*k] = new_increments;
	}
}

void TimeControl::set_increment(int new_increment, const Side &side) {
	m_increment[side] = new_increment;
}

bool TimeControl::both_sides_have_same_time() const {
	bool retval = m_main_time[LEFT]==m_main_time[RIGHT];
	if(m_mode==FISCHER || m_mode==BRONSTEIN)
		retval = retval && (m_increment[LEFT]==m_increment[RIGHT]);
	return retval;
}

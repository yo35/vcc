/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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


#include "bitimer.h"
#include <cassert>


// Constructeur
BiTimer::BiTimer() : Glib::Object() {
	reset_timers();
}

// Accesseurs
BiTimer::Mode BiTimer::mode() const {
	return m_mode;
}

Side BiTimer::active_side() const {
	assert(m_mode==ACTIVE);
	return m_active_side;
}

const TimeControl &BiTimer::time_control() const {
	return m_time_control;
}

const Timer &BiTimer::timer(const Side &side) const {
	return m_timer[side];
}

// Démarre un timer
void BiTimer::start_timer(const Side &side) {
	assert(m_mode==PAUSED);
	if(m_time_control.mode()==HOURGLASS)
		m_timer[rev(side)].set_mode(Timer::INCREMENT);
	m_timer[side].set_mode(Timer::DECREMENT);
	m_active_side = side;
	m_mode        = ACTIVE;
}

// Modifie le camp au trait
void BiTimer::change_timer() {
	assert(m_mode==ACTIVE);

	// En hour-glass, le timer inactif est en mode incrément
	if(m_time_control.mode()==HOURGLASS)
		m_timer[m_active_side].set_mode(Timer::INCREMENT);

	// Sinon, il est mis en pause
	else {
		m_timer[m_active_side].set_mode(Timer::PAUSED);

		// En mode fischer ou bronstein, on incrémente le compteur à chaque coup,
		// Rq : uniquement si le compteur est positif
		if((
			m_time_control.mode()==FISCHER ||
			m_time_control.mode()==BRONSTEIN) &&
			m_timer[m_active_side].get_time() >= 0)
		{
			int new_time = m_timer[m_active_side].get_time()
				+ m_time_control.increment(m_active_side);
			if(m_time_control.mode()==BRONSTEIN) {
				if(new_time > m_bronstein_limit[m_active_side])
					new_time = m_bronstein_limit[m_active_side];
				else
					m_bronstein_limit[m_active_side] = new_time;
			}
			m_timer[m_active_side].set_time(new_time);
		}
	}

	// Le nouveau timer actif passe en mode décrement
	m_active_side = rev(m_active_side);
	m_timer[m_active_side].set_mode(Timer::DECREMENT);
}

// Arrête la pendule
void BiTimer::stop_timer() {
	assert(m_mode==ACTIVE);
	m_timer[LEFT ].set_mode(Timer::PAUSED);
	m_timer[RIGHT].set_mode(Timer::PAUSED);
	m_mode = PAUSED;
}

// Réinitialise le tout
void BiTimer::reset_timers() {
	for(Side::iterator k=Side::first(); k.valid(); ++k) {

		// Mise en pause
		m_timer[*k].set_mode(Timer::PAUSED);

		// Calcul du temps initial
		int start_time = m_time_control.main_time(*k);
		if(m_time_control.mode()==FISCHER
			|| m_time_control.mode()==BRONSTEIN)
		{
			start_time += m_time_control.increment(*k);
		}

		// Définition des flags
		m_timer[*k].set_time(start_time);
		if(m_time_control.mode()==BRONSTEIN)
			m_bronstein_limit[*k] = start_time;
	}
	m_mode = PAUSED;
}

// Modifie la cadence
void BiTimer::set_time_control(const TimeControl &time_control) {
	m_time_control = time_control;
	reset_timers();
}

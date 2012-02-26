/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


#include "eventdelayer.h"
#include <cassert>
#include <glibmm/main.h>


// Constructeur
EventDelayer::EventDelayer(unsigned int nb_trigger) : Glib::Object(),
	m_is_active   (nb_trigger),
	m_triggered_at(nb_trigger)
{
	// Initialisation des variables internes
	m_delay = 0;
	for(unsigned int i=0; i<nb_trigger; ++i) {
		m_is_active[i] = false;
	}

	// Routine de check
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &EventDelayer::on_timeout_elapses), 100);
}

int EventDelayer::delay() const {
	return m_delay;
}

void EventDelayer::set_delay(int src) {
	m_delay = src;
}

sigc::signal<void> EventDelayer::signal_occurred() const {
	return m_signal_occurred;
}

// Activation d'un trigger
void EventDelayer::trigger(unsigned int no) {
	assert(no < m_is_active.size());
	if(m_is_active[no])
		return;
	m_is_active   [no] = true;
	m_triggered_at[no] = get_timestamp_now();
}

// Désactivation d'un trigger
void EventDelayer::cancel_trigger(unsigned int no) {
	assert(no < m_is_active.size());
	m_is_active[no] = false;
}

// Routine de check
bool EventDelayer::on_timeout_elapses() {

	// On vérifie que tous les triggers sont actifs
	for(unsigned int i=0; i<m_is_active.size(); ++i) {
		if(!m_is_active[i])
			return true;
	}

	// On vérifie que tous ont été déclenchés depuis suffisamment longtemps
	Timestamp now = get_timestamp_now();
	for(unsigned int i=0; i<m_triggered_at.size(); ++i) {
		int triggered_since = difftime(now, m_triggered_at[i]);
		if(triggered_since < m_delay)
			return true;
	}

	// On déclenche l'événement, et on réset tous les triggers
	for(unsigned int i=0; i<m_is_active.size(); ++i) {
		m_is_active[i] = false;
	}
	m_signal_occurred.emit();
	return true;
}

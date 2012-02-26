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


#ifndef EVENTDELAYER_H
#define EVENTDELAYER_H

#include <glibmm/object.h>
#include "timestamp.h"
#include <vector>

class EventDelayer : public Glib::Object {

public:

	// Constructeur et gestion du délai
	EventDelayer(unsigned int nb_trigger);
	int delay() const;
	void set_delay(int src);

	// Signal déclenché
	sigc::signal<void> signal_occurred() const;

	// Activation/désactivation d'un trigger
	void trigger(unsigned int no);
	void cancel_trigger(unsigned int no);

private:

	// Routine de check
	bool on_timeout_elapses();

	// Données membres
	int                    m_delay          ;
	std::vector<bool>      m_is_active      ;
	std::vector<Timestamp> m_triggered_at   ;
	sigc::signal<void>     m_signal_occurred;
};

#endif

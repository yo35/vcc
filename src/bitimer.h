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


#ifndef BITIMER_H
#define BITIMER_H

#include <glibmm/object.h>
#include "side.h"
#include "timer.h"
#include "timecontrol.h"


// Double-timer
class BiTimer : public Glib::Object {

public:

	// Mode du double-timer
	typedef enum {
		ACTIVE,
		PAUSED
	} Mode;

	// Divers
	BiTimer();
	sigc::signal<void> signal_state_changed() const;

	// Accesseurs
	Mode               mode        () const;
	Side               active_side () const;
	const TimeControl &time_control() const;
	int get_time(const Side &side) const;

	// Modifie le cadran actif
	void start_timer(const Side &side);
	void change_timer();
	void stop_timer();

	// Remets les timers à zéro
	void reset_timers();

	// Modifie la cadence
	void set_time_control(const TimeControl &time_control);

private:

	// Alias
	typedef EnumArray<Side, Timer> TimerSideArray;

	// Données membres
	Mode               m_mode                ;
	Side               m_active_side         ;
	TimeControl        m_time_control        ;
	TimerSideArray     m_timer               ;
	IntSideArray       m_bronstein_limit     ;
	sigc::signal<void> m_signal_state_changed;
};

#endif

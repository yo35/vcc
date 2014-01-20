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


#ifndef BITIMER_H_
#define BITIMER_H_

#include "side.h"
#include "timecontrol.h"
#include "timer.h"
#include <signals.h>
#include <boost/optional.hpp>


/**
 * Two timers whose behaviors are defined and coordinated by a time control object.
 */
class BiTimer
{
public:

	/**
	 * Constructor.
	 */
	BiTimer() { reset_timers(); }

	/**
	 * @name Copy is not allowed.
	 * @{
	 */
	BiTimer(const BiTimer &op) = delete;
	BiTimer &operator=(const BiTimer &op) = delete;
	/**@} */

	/**
	 * Signal sent when the state of the timer pair changes.
	 */
	sig::connection connect_state_changed(const sig::signal<void()>::slot_type &slot) const
	{
		return _signal_state_changed.connect(slot);
	}

	/**
	 * Check whether one of the side is active, or if both timers are paused.
	 */
	bool is_active() const { return _active_side; }

	/**
	 * Return the active side, or `boost::none` if both timers are paused.
	 */
	const boost::optional<Side> &active_side() const { return _active_side; }

	/**
	 * Return the time control.
	 */
	const TimeControl &time_control() const { return _time_control; }

	/**
	 * Change the current time control. A call to this function automatically
	 * stops and resets the timers.
	 */
	void set_time_control(TimeControl time_control);

	/**
	 * Current time of the timer on side `side`.
	 */
	TimeDuration time(Side side) const { return _timer[side].time(); }

	/**
	 * Current time, with additional information about the bronstein delay
	 * \throw RuntimeException If time_control().mode()!=BRONSTEIN
	 */
	//TimeDuration time_bronstein(Side side, TimeDuration &bronstein_extra_delay) const; //TODO

	/**
	 * Start the timer corresponding to side `side`.
	 *
	 * If `side` is already active, nothing happens. If the opposite timer is active,
	 * the method `change_timer()` is called.
	 */
	void start_timer(Side side);

	/**
	 * Change the active side. Nothing happens if both timers are paused.
	 */
	void change_timer();

	/**
	 * Stop the active timer. Nothing happens if both timers are paused.
	 */
	void stop_timer();

	/**
	 * Reset the timers. A call to this function automatically stops the timers.
	 */
	void reset_timers();

private:

	// Private functions
	TimeDuration initial_time(Side side) const;

	// Private members
	mutable sig::signal<void()>     _signal_state_changed;
	boost::optional<Side>           _active_side         ;
	TimeControl                     _time_control        ;
	Enum::array<Side, Timer>        _timer               ;
	Enum::array<Side, TimeDuration> _bronstein_limit     ;
};

#endif /* BITIMER_H_ */

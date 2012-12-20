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

#include "common.h"
#include "timecontrol.h"
#include "timer.h"
#include "enumarray.h"
#include <boost/optional.hpp>
#include <boost/signals.hpp>

/**
 * Two timers whose behaviors are defined and coordinated by a time control object
 */
class BiTimer
{
public:

	/**
	 * Constructor
	 */
	BiTimer();

	/**
	 * \name Copy is not allowed
	 * @{
	 */
	BiTimer(const BiTimer &op) = delete;
	BiTimer &operator=(const BiTimer &op) = delete;
	///@}

	/**
	 * Signal sent when the state of the timer pair changes
	 */
	boost::signals::connection connect_state_changed(const boost::signal<void()>::slot_type &slot) const;

	/**
	 * Return the active side, or \p boost::none if both timers are paused
	 */
	const boost::optional<Side> &active_side() const;

	/**
	 * Current time control
	 */
	const TimeControl &time_control() const;

	/**
	 * Change the current time control
	 * \remarks A call to this function automatically resets and stops the timers
	 */
	void set_time_control(TimeControl time_control);

	/**
	 * Current time of the timer of side \p side
	 */
	TimeDuration time(Side side) const;

	/**
	 * Current time, with additional information about the bronstein delay
	 * \throw RuntimeException If time_control().mode()!=BRONSTEIN
	 */
	TimeDuration time_bronstein(Side side, TimeDuration &bronstein_extra_delay) const;

	/**
	 * Start the timer corresponding to side \p side
	 * \remarks If \p side is already active, nothing happens. If the opposite timer
	 *          is active, the method \p change_timer() is called.
	 */
	void start_timer(Side side);

	/**
	 * Change the active side
	 * \remarks Nothing happens if both timers are paused
	 */
	void change_timer();

	/**
	 * Stop the active timer
	 * \remarks Nothing happens if both timers are paused
	 */
	void stop_timer();

	/**
	 * Reset the timers
	 * \remarks A call to this function automatically stops the timers
	 */
	void reset_timers();

private:

	// Private functions
	TimeDuration initial_time(Side side) const;

	// Private data
	mutable boost::signal<void()> m_signal_state_changed;
	boost::optional<Side>         m_active_side         ;
	TimeControl                   m_time_control        ;
	EnumArray<Side, Timer>        m_timer               ;
	EnumArray<Side, TimeDuration> m_bronstein_limit     ;
};

#endif /* BITIMER_H_ */

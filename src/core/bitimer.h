/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2014 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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
#include <wrappers/signals.h>
#include <boost/optional.hpp>


/**
 * Two timers whose behaviors are defined and coordinated by a time control object.
 */
class BiTimer
{
public:


	/**
	 * Structure used to return detailed information about the time of a given side.
	 */
	struct TimeInfo
	{
		TimeDuration total_time        ; //!< Total remaining time.
		TimeDuration main_time         ; //!< Equal to the total time, except in Bronstein and byo-yomi modes.
		TimeDuration bronstein_time    ; //!< In Bronstein mode, remaining time before the main time starts to decrease.
		int          current_byo_period; //!< In byo-yomi mode, number of the current byo-period (0 for the main-time period).
		int          total_byo_periods ; //!< In byo-yomi mode, total number of byo-periods.

		/**
		 * Factory method for "standard" time control modes.
		 */
		static TimeInfo make(const TimeDuration &tt) { return TimeInfo(tt, TimeDuration::zero(), TimeDuration::zero(), 0, 0); }

		/**
		 * Factory method for Bronstein mode.
		 */
		static TimeInfo makeBronstein(const TimeDuration &tt, const TimeDuration &mt, const TimeDuration &bt)
		{
			return TimeInfo(tt, mt, bt, 0, 0);
		}

		/**
		 * Factory method for byo-yomi mode.
		 */
		static TimeInfo makeByoYomi(const TimeDuration &tt, const TimeDuration &mt, int cbp, int tbp)
		{
			return TimeInfo(tt, mt, TimeDuration::zero(), cbp, tbp);
		}

	private:

		// Constructor.
		explicit TimeInfo(const TimeDuration &tt, const TimeDuration &mt, const TimeDuration &bt, int cbp, int tbp) :
			total_time(tt), main_time(mt), bronstein_time(bt), current_byo_period(cbp), total_byo_periods(tbp)
		{}
	};


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
	 * Current time of the timer on side `side`, with additional information.
	 */
	TimeInfo detailed_time(Side side) const;

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

	/**
	 * Swap the sides. The active side may change if any.
	 */
	void swap_sides();

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

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


#ifndef TIMER_H_
#define TIMER_H_

#include <cstdint>
#include "chrono.h"


/**
 * Stoppable and reversible clock.
 */
class Timer
{
public:

	/**
	 * Timer behavior.
	 */
	enum class Mode : std::uint8_t
	{
		INCREMENT, //!< Time is incrementing.
		DECREMENT, //!< Time is decrementing.
		PAUSED     //!< The timer is stopped.
	};

	/**
	 * Constructor.
	 */
	Timer() : _mode(Mode::PAUSED) {}

	/**
	 * Timer behavior.
	 */
	Mode mode() const { return _mode; }

	/**
	 * Change the behavior of the timer.
	 */
	void set_mode(Mode mode);

	/**
	 * Current time.
	 */
	TimeDuration time() const;

	/**
	 * Change the current time. A call to this function stops the timer
	 * (its mode is set to `Mode::PAUSED`).
	 */
	void set_time(TimeDuration time);

private:

	// Private members
	Mode         _mode    ;
	TimeDuration _time    ;
	TimePoint    _start_at;
};

#endif /* TIMER_H_ */

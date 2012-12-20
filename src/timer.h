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


#ifndef TIMER_H_
#define TIMER_H_

#include "common.h"

/**
 * Stoppable and reversible clock
 */
class Timer
{
public:

	/**
	 * Timer behavior
	 */
	enum class Mode : char
	{
		INCREMENT, //!< Time is incrementing
		DECREMENT, //!< Time is decrementing
		PAUSED     //!< Time does not change
	};

	/**
	 * Constructor
	 */
	Timer();

	/**
	 * \name Copy is not allowed
	 * @{
	 */
	Timer(const Timer &op) = delete;
	Timer &operator=(const Timer &op) = delete;
	///@}

	/**
	 * Timer behavior
	 */
	Mode mode() const;

	/**
	 * Change the behavior
	 */
	void set_mode(Mode mode);

	/**
	 * Current time
	 */
	TimeDuration time() const;

	/**
	 * Change the current time
	 * \remarks This function will stop the timer (its mode will be set to \p PAUSED)
	 */
	void set_time(TimeDuration time);

private:

	// Member objects
	Mode         m_mode    ;
	TimeDuration m_time    ;
	TimePoint    m_start_at;
};

#endif /* TIMER_H_ */

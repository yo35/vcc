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


#include "timer.h"

/**
 * Constructor
 */
Timer::Timer()
{
	m_mode = Mode::PAUSED;
}

/**
 * Timer behavior
 */
Timer::Mode Timer::mode() const
{
	return m_mode;
}

/**
 * Change the behavior
 */
void Timer::set_mode(Mode mode)
{
	if(m_mode==mode) {
		return;
	}
	if(m_mode!=Mode::PAUSED) {
		m_time = time();
	}
	if(mode!=Mode::PAUSED) {
		m_start_at = boost::posix_time::microsec_clock::local_time();
	}
	m_mode = mode;
}

/**
 * Current time
 */
TimeDuration Timer::time() const
{
	if(m_mode==Mode::PAUSED) {
		return m_time;
	}
	else {
		TimePoint    now  = current_time();
		TimeDuration diff = (now - m_start_at);
		if(m_mode==Mode::INCREMENT)
			return m_time + diff;
		else // m_mode==Mode::DECREMENT
			return m_time - diff;
	}
}

/**
 * Change the current time, and stop the timer if it is not already stopped
 */
void Timer::set_time(TimeDuration time)
{
	m_mode = Mode::PAUSED;
	m_time = std::move(time);
}

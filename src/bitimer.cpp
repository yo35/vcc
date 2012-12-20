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


#include "bitimer.h"
#include <translation.h>
#include <exception.h>

/**
 * Constructor
 */
BiTimer::BiTimer()
{
	reset_timers();
}

/**
 * Signal sent when the state of the timer pair changes
 */
boost::signals::connection BiTimer::connect_state_changed(const boost::signal<void()>::slot_type &slot) const
{
	return m_signal_state_changed.connect(slot);
}

/**
 * Return the active side, or \p boost::none if both timers are paused
 */
const boost::optional<Side> &BiTimer::active_side() const
{
	return m_active_side;
}

/**
 * Current time control
 */
const TimeControl &BiTimer::time_control() const
{
	return m_time_control;
}

/**
 * Change the current time control
 */
void BiTimer::set_time_control(TimeControl time_control)
{
	m_time_control = std::move(time_control);
	reset_timers();
}

/**
 * Current time of the timer of side \p side
 */
TimeDuration BiTimer::time(Side side) const
{
	return m_timer[side].time();
}

/**
 * Current time, with additional information about the bronstein delay
 */
TimeDuration BiTimer::time_bronstein(Side side, TimeDuration &bronstein_extra_delay) const
{
	if(m_time_control.mode()!=TimeControl::BRONSTEIN) {
		throw RuntimeException(_("No Bronstein delay information with the current time control mode"));
	}
	TimeDuration retval = m_timer[side].time();
	bronstein_extra_delay = retval - m_bronstein_limit[side] + m_time_control.increment(side);
	return retval;
}

/**
 * Start the timer corresponding to side \p side
 */
void BiTimer::start_timer(Side side)
{
	// Deal with the situation where one of the timers is already running
	if(m_active_side) {
		if(*m_active_side!=side) {
			change_timer();
		}
		return;
	}

	// Regular situation
	if(m_time_control.mode()==TimeControl::HOURGLASS && !m_timer[reverse(side)].time().is_negative()) {
		m_timer[reverse(side)].set_mode(Timer::Mode::INCREMENT);
	}
	m_timer[side].set_mode(Timer::Mode::DECREMENT);
	m_active_side = side;
	m_signal_state_changed();
}

/**
 * Change the active side
 */
void BiTimer::change_timer()
{
	// Nothing to do if no timer is running
	if(!m_active_side) {
		return;
	}

	// Regular situation
	TimeControl::Mode current_mode = m_time_control.mode();
	Side              active_side  = *m_active_side;
	TimeDuration      current_time = m_timer[active_side].time();

	// With hour-glass mode, the future "inactive" timer is incrementing
	if(current_mode==TimeControl::HOURGLASS && !current_time.is_negative()) {
		m_timer[active_side].set_mode(Timer::Mode::INCREMENT);
	}

	// Otherwise, it must be stopped
	else {
		m_timer[active_side].set_mode(Timer::Mode::PAUSED);

		// If the current player still has time, his/her timer must be incremented
		if(!current_time.is_negative()) {
			TimeDuration new_time = current_time;

			// Fischer mode => grant unconditionally the increment to the player
			if(current_mode==TimeControl::FISCHER) {
				new_time = current_time + m_time_control.increment(active_side);
			}

			// Bronstein mode => grant the increment, but clamp it to the Bronstein's threshold
			else if(current_mode==TimeControl::BRONSTEIN) {
				new_time = current_time + m_time_control.increment(active_side);
				if(new_time > m_bronstein_limit[active_side])
					new_time = m_bronstein_limit[active_side];
				else
					m_bronstein_limit[active_side] = new_time;
			}

			// Byo-yomi => detect if the player is currently in one of the final byo-periods,
			// and adjust the remaining time if necessary
			else if(current_mode==TimeControl::BYO_YOMI) {
				long increment = m_time_control.increment(active_side).total_milliseconds();
				if(increment>0) {
					int current_byo_period = current_time.total_milliseconds()/increment;
					if(current_byo_period < m_time_control.byo_periods(active_side)) {
						new_time = m_time_control.increment(active_side) * (current_byo_period+1);
					}
				}
			}
			m_timer[active_side].set_time(new_time);
		}
	}

	// The new active timer is now decrementing
	active_side = reverse(active_side);
	m_timer[active_side].set_mode(Timer::Mode::DECREMENT);
	m_active_side = active_side;
	m_signal_state_changed();
}

/**
 * Stop the active timer
 */
void BiTimer::stop_timer()
{
	if(!m_active_side) {
		return;
	}
	m_timer[LEFT ].set_mode(Timer::Mode::PAUSED);
	m_timer[RIGHT].set_mode(Timer::Mode::PAUSED);
	m_active_side = boost::none;
	m_signal_state_changed();
}

/**
 * Reset the timers
 */
void BiTimer::reset_timers()
{
	// Stop the timers
	m_timer[LEFT ].set_mode(Timer::Mode::PAUSED);
	m_timer[RIGHT].set_mode(Timer::Mode::PAUSED);

	// Set the initial time
	m_timer[LEFT ].set_time(initial_time(LEFT ));
	m_timer[RIGHT].set_time(initial_time(RIGHT));
	if(m_time_control.mode()==TimeControl::BRONSTEIN) {
		m_bronstein_limit[LEFT ] = m_timer[LEFT ].time();
		m_bronstein_limit[RIGHT] = m_timer[RIGHT].time();
	}

	// Update the state flag and fire the signal
	m_active_side = boost::none;
	m_signal_state_changed();
}

/**
 * Return the initial time to allocate to the given timer
 */
TimeDuration BiTimer::initial_time(Side side) const
{
	TimeDuration      retval       = m_time_control.main_time(side);
	TimeControl::Mode current_mode = m_time_control.mode();
	if(current_mode==TimeControl::FISCHER || current_mode==TimeControl::BRONSTEIN) {
		retval += m_time_control.increment(side);
	}
	else if(current_mode==TimeControl::BYO_YOMI) {
		retval += m_time_control.increment(side) * m_time_control.byo_periods(side);
	}
	return retval;
}

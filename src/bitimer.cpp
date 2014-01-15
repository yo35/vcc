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


// Change the current time control, and resets the timers.
void BiTimer::set_time_control(TimeControl time_control)
{
	_time_control = std::move(time_control);
	reset_timers();
}

#if 0

/**
 * Current time, with additional information about the bronstein delay
 */
TimeDuration BiTimer::time_bronstein(Side side, TimeDuration &bronstein_extra_delay) const
{
	if(m_time_control.mode()!=TimeControl::BRONSTEIN) {
		throw RuntimeException("No Bronstein delay information with the current time control mode");
	}
	TimeDuration retval = m_timer[side].time();
	bronstein_extra_delay = retval - m_bronstein_limit[side] + m_time_control.increment(side);
	return retval;
}

#endif


// Start a timer.
void BiTimer::start_timer(Side side)
{
	// Deal with the situation where one of the timers is already running
	if(_active_side) {
		if(*_active_side!=side) {
			change_timer();
		}
		return;
	}

	// Regular situation
	if(_time_control.mode()==TimeControl::Mode::HOURGLASS && _timer[flip(side)].time()>=TimeDuration::zero()) {
		_timer[flip(side)].set_mode(Timer::Mode::INCREMENT);
	}
	_timer[side].set_mode(Timer::Mode::DECREMENT);
	_active_side = side;
	_signal_state_changed();
}


// Change the active side
void BiTimer::change_timer()
{
	// Nothing to do if no timer is running
	if(!_active_side) {
		return;
	}

	// Regular situation
	TimeControl::Mode current_mode = _time_control.mode();
	Side              active_side  = *_active_side;
	TimeDuration      current_time = _timer[active_side].time();

	// With hour-glass mode, the future "inactive" timer is incrementing
	if(current_mode==TimeControl::Mode::HOURGLASS && current_time>=TimeDuration::zero()) {
		_timer[active_side].set_mode(Timer::Mode::INCREMENT);
	}

	// Otherwise, it must be stopped
	else {
		_timer[active_side].set_mode(Timer::Mode::PAUSED);

		// If the current player still has time, his/her timer may be incremented,
		// depending on the time control mode.
		if(current_time>=TimeDuration::zero()) {
			TimeDuration new_time = current_time;

			// Fischer mode => grant unconditionally the increment to the player
			if(current_mode==TimeControl::Mode::FISCHER) {
				new_time = current_time + _time_control.increment(active_side);
			}

			// Bronstein mode => grant the increment, but clamp it to the Bronstein's threshold
			else if(current_mode==TimeControl::Mode::BRONSTEIN) {
				new_time = current_time + _time_control.increment(active_side);
				if(new_time > _bronstein_limit[active_side]) {
					new_time = _bronstein_limit[active_side];
				}
				else {
					_bronstein_limit[active_side] = new_time;
				}
			}

			// Byo-yomi => detect if the player is currently in one of the final byo-periods,
			// and adjust the remaining time if necessary
			else if(current_mode==TimeControl::Mode::BYO_YOMI) {
				TimeDuration increment = _time_control.increment(active_side);
				if(increment>TimeDuration::zero()) {
					int current_byo_period = current_time/increment;
					if(current_byo_period < _time_control.byo_periods(active_side)) {
						new_time = increment * (current_byo_period+1);
					}
				}
			}

			// Set the incremented time.
			_timer[active_side].set_time(new_time);
		}
	}

	// The new active timer is now decrementing
	active_side = flip(active_side);
	_timer[active_side].set_mode(Timer::Mode::DECREMENT);
	_active_side = active_side;
	_signal_state_changed();
}


// Stop the active timer.
void BiTimer::stop_timer()
{
	if(!_active_side) {
		return;
	}
	_timer[Side::LEFT ].set_mode(Timer::Mode::PAUSED);
	_timer[Side::RIGHT].set_mode(Timer::Mode::PAUSED);
	_active_side = boost::none;
	_signal_state_changed();
}


// Stop and reset the timers.
void BiTimer::reset_timers()
{
	// Stop the timers
	_timer[Side::LEFT ].set_mode(Timer::Mode::PAUSED);
	_timer[Side::RIGHT].set_mode(Timer::Mode::PAUSED);

	// Set the initial time
	_timer[Side::LEFT ].set_time(initial_time(Side::LEFT ));
	_timer[Side::RIGHT].set_time(initial_time(Side::RIGHT));
	if(_time_control.mode()==TimeControl::Mode::BRONSTEIN) {
		_bronstein_limit[Side::LEFT ] = _timer[Side::LEFT ].time();
		_bronstein_limit[Side::RIGHT] = _timer[Side::RIGHT].time();
	}

	// Update the state flag and fire the signal
	_active_side = boost::none;
	_signal_state_changed();
}


// Return the initial time to allocate to the given timer.
TimeDuration BiTimer::initial_time(Side side) const
{
	TimeDuration      retval       = _time_control.main_time(side);
	TimeControl::Mode current_mode = _time_control.mode();
	if(current_mode==TimeControl::Mode::FISCHER || current_mode==TimeControl::Mode::BRONSTEIN) {
		retval += _time_control.increment(side);
	}
	else if(current_mode==TimeControl::Mode::BYO_YOMI) {
		retval += _time_control.increment(side) * _time_control.byo_periods(side);
	}
	return retval;
}

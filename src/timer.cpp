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
#include <utility>


// Change the behavior of the timer.
void Timer::set_mode(Mode mode)
{
	if(_mode==mode) {
		return;
	}
	if(_mode!=Mode::PAUSED) {
		_time = time();
	}
	if(mode!=Mode::PAUSED) {
		_start_at = current_time();
	}
	_mode = mode;
}


// Current time.
TimeDuration Timer::time() const
{
	if(_mode==Mode::PAUSED) {
		return _time;
	}
	else {
		TimePoint    now  = current_time();
		TimeDuration diff = now - _start_at;
		if(_mode==Mode::INCREMENT)
			return _time + diff;
		else // _mode==Mode::DECREMENT
			return _time - diff;
	}
}


// Change the current time, and stop the timer if it is not already stopped.
void Timer::set_time(TimeDuration time)
{
	_mode = Mode::PAUSED;
	_time = std::move(time);
}

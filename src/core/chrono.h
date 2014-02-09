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


#ifndef CHRONO_H_
#define CHRONO_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdlib>


/**
 * A time interval, with a resolution of one millisecond.
 */
typedef boost::posix_time::time_duration TimeDuration;


/**
 * A reference to a point in the time.
 */
typedef boost::posix_time::ptime TimePoint;


/**
 * Return the current time point.
 */
inline TimePoint current_time()
{
	return boost::posix_time::microsec_clock::local_time();
}


/**
 * Return the TimeDuration object corresponding to the given number of seconds.
 */
inline TimeDuration from_seconds(long sec)
{
	return boost::posix_time::seconds(sec);
}


/**
 * Extract the total (rounded) number of seconds represented by a TimeDuration object.
 */
inline long to_seconds(const TimeDuration &td)
{
	auto retval = std::div(td.total_milliseconds(), 1000l);
	return retval.quot + (retval.rem>=500 ? 1 : (retval.rem<-500 ? -1: 0));
}


/**
 * Zero-length time duration.
 */
const TimeDuration TIME_DURATION_ZERO = from_seconds(0);


/**
 * Division operator between two TimeDuration objects.
 */
inline long operator/(const TimeDuration &lhs, const TimeDuration &rhs)
{
	return lhs.total_milliseconds() / rhs.total_milliseconds();
}


#endif /* CHRONO_H_ */

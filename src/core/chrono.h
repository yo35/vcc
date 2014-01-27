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

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <istream>


/**
 * A time interval, with a resolution of one millisecond.
 */
typedef std::chrono::duration<std::int64_t, std::milli> TimeDuration;


/**
 * A reference to a point in the time.
 */
typedef std::chrono::time_point<std::chrono::high_resolution_clock, TimeDuration> TimePoint;


/**
 * Return the current time point.
 */
inline TimePoint current_time()
{
	return std::chrono::time_point_cast<TimePoint::duration>(std::chrono::high_resolution_clock::now());
}


/**
 * Return the TimeDuration object corresponding to the given number of seconds.
 */
inline TimeDuration from_seconds(long sec)
{
	return TimeDuration(static_cast<TimeDuration::rep>(sec)*static_cast<TimeDuration::rep>(1000));
}


/**
 * Extract the total (rounded) number of seconds represented by a TimeDuration object.
 */
inline long to_seconds(const TimeDuration &td)
{
	auto retval = std::div(td.count(), static_cast<TimeDuration::rep>(1000));
	return retval.quot + (retval.rem>=500 ? 1 : (retval.rem<-500 ? -1: 0));
}


/**
 * Output stream operator for time duration values.
 */
inline std::ostream &operator<<(std::ostream &stream, TimeDuration td)
{
	return stream << td.count();
}


/**
 * Input stream operator for time duration values.
 */
inline std::istream &operator>>(std::istream &stream, TimeDuration &td)
{
	TimeDuration::rep buffer = 0;
	stream >> buffer;
	td = TimeDuration(buffer);
	return stream;
}


#endif /* CHRONO_H_ */

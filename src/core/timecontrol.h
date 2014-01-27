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


#ifndef TIMECONTROL_H_
#define TIMECONTROL_H_

#include <cstdint>
#include <string>
#include <ostream>
#include "enumutil.h"
#include "side.h"
#include "chrono.h"


/**
 * Type of time control.
 * @remarks Prefer using the alias `TimeControl::Mode`.
 */
enum class _TimeControlMode : std::uint8_t
{
	SUDDEN_DEATH,
	FISCHER     ,
	BRONSTEIN   ,
	HOURGLASS   ,
	BYO_YOMI
};

namespace Enum { template<> struct traits<_TimeControlMode> : trait_indexing<5> {}; }



/**
 * Object describing how time is affected to players in a chess/go/backgammon game,
 * and when they are out of time.
 */
class TimeControl
{
public:

	/**
	 * Type of time control.
	 */
	typedef _TimeControlMode Mode;

	/**
	 * Name of a time control mode.
	 */
	static const std::string &mode_name(Mode mode);

	/**
	 * Constructor.
	 */
	TimeControl() : _mode(Mode::SUDDEN_DEATH), _byo_periods{0, 0} {}

	/**
	 * Current time control mode.
	 */
	Mode mode() const { return _mode; }

	/**
	 * Change the time control mode.
	 */
	void set_mode(Mode mode) { _mode=mode; }

	/**
	 * Main time.
	 */
	const TimeDuration &main_time(Side side) const { return _main_time[side]; }

	/**
	 * Set the main time.
	 * @throw std::invalid_argument If `value` represents a negative time duration.
	 */
	void set_main_time(Side side, TimeDuration value);

	/**
	 * Increment.
	 */
	const TimeDuration &increment(Side side) const { return _increment[side]; }

	/**
	 * Set the increment
	 * @throw std::invalid_argument If `value` represents a negative time duration.
	 */
	void set_increment(Side side, TimeDuration value);

	/**
	 * Number of byo-yomi periods.
	 */
	int byo_periods(Side side) const { return _byo_periods[side]; }

	/**
	 * Set the number of byo-yomi periods
	 * @throw std::invalid_argument If `value<0`.
	 */
	void set_byo_periods(Side side, int value);

	/**
	 * Check whether both sides have the same time parameters.
	 */
	bool both_sides_have_same_time() const;

	/**
	 * Swap the side-related options.
	 */
	void swap_sides();

	/**
	 * Name of the current time control mode.
	 */
	const std::string &mode_name() const;

	/**
	 * Human-readable description of the time-control.
	 */
	std::string description() const;

private:

	// Private functions
	void side_description(std::ostream &stream, Side side) const;
	static void format_time(std::ostream &stream, const TimeDuration &value);

	// Private members
	Mode                            _mode       ;
	Enum::array<Side, TimeDuration> _main_time  ;
	Enum::array<Side, TimeDuration> _increment  ;
	Enum::array<Side, int         > _byo_periods;
};

#endif /* TIMECONTROL_H_ */

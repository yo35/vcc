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


#ifndef TIMECONTROL_H_
#define TIMECONTROL_H_

#include "common.h"
#include "enumerable.h"
#include "enumarray.h"
#include <string>
#include <ostream>

/**
 * Object describing how time is affected to players in a chess/go/backgammon
 * game, and when they are out of time
 */
class TimeControl
{
	struct _Mode { static const std::size_t N = 5; };

public:

	/**
	 * Type of time control
	 */
	typedef Enumerable<_Mode> Mode;

	/**
	 * \name Available time control modes
	 * @{
	 */
	static const Mode SUDDEN_DEATH;
	static const Mode FISCHER     ;
	static const Mode BRONSTEIN   ;
	static const Mode HOURGLASS   ;
	static const Mode BYO_YOMI    ;
	///@}

	/**
	 * Name of a time control mode
	 */
	static const std::string &mode_name(Mode mode);

	/**
	 * Constructor
	 */
	TimeControl();

	/**
	 * Current time control mode
	 */
	Mode mode() const;

	/**
	 * Change the current time control mode
	 */
	void set_mode(Mode mode);

	/**
	 * Main time
	 */
	const TimeDuration &main_time(Side side) const;

	/**
	 * Set the main time
	 * \throw RuntimeException If value<0
	 */
	void set_main_time(Side side, TimeDuration value);

	/**
	 * Increment
	 */
	const TimeDuration &increment(Side side) const;

	/**
	 * Set the increment
	 * \throw RuntimeException If value<0
	 */
	void set_increment(Side side, TimeDuration value);

	/**
	 * Number of byo-yomi periods
	 */
	int byo_periods(Side side) const;

	/**
	 * Set the number of byo-yomi periods
	 * \throw RuntimeException If value<0
	 */
	void set_byo_periods(Side side, int value);

	/**
	 * Check whether both sides have the same time parameters
	 */
	bool both_sides_have_same_time() const;

	/**
	 * Name of the current time control mode
	 */
	const std::string &mode_name() const;

	/**
	 * String describing the current time control
	 */
	std::string description() const;

	/**
	 * Output in a stream
	 */
	friend std::ostream &operator<<(std::ostream &lhs, const TimeControl &rhs);

private:

	// Private functions
	void side_description(std::ostream &stream, Side side) const;
	static void format_time(std::ostream &stream, const TimeDuration &value);

	// Hold the state information associated to a given side
	struct SideState
	{
		TimeDuration main_time  ;
		TimeDuration increment  ;
		int          byo_periods;
		SideState() : byo_periods(0) {}
	};

	// Private data
	Mode                       m_mode ;
	EnumArray<Side, SideState> m_state;
};

#endif /* TIMECONTROL_H_ */

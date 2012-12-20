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


#include "timecontrol.h"
#include <translation.h>
#include <exception.h>
#include <sstream>
#include <boost/format.hpp>

// Available time control modes
const TimeControl::Mode TimeControl::SUDDEN_DEATH(0);
const TimeControl::Mode TimeControl::FISCHER     (1);
const TimeControl::Mode TimeControl::BRONSTEIN   (2);
const TimeControl::Mode TimeControl::HOURGLASS   (3);
const TimeControl::Mode TimeControl::BYO_YOMI    (4);

/**
 * Name of a time control mode
 */
const std::string &TimeControl::mode_name(Mode mode)
{
	static bool do_init = true;
	static EnumArray<Mode, std::string> retval;
	if(do_init) {
		retval[SUDDEN_DEATH] = _("Sudden death");
		retval[FISCHER     ] = _("Fischer"     );
		retval[BRONSTEIN   ] = _("Bronstein"   );
		retval[HOURGLASS   ] = _("Hourglass"   );
		retval[BYO_YOMI    ] = _("Byo-yomi"    );
		do_init = false;
	}
	return retval[mode];
}

/**
 * Constructor
 */
TimeControl::TimeControl()
{
	m_mode = SUDDEN_DEATH;
}

/**
 * Current time control mode
 */
TimeControl::Mode TimeControl::mode() const
{
	return m_mode;
}

/**
 * Change the current time control mode
 */
void TimeControl::set_mode(Mode mode)
{
	m_mode = mode;
}

// Getters
const TimeDuration &TimeControl::main_time  (Side side) const { return m_state[side].main_time  ; }
const TimeDuration &TimeControl::increment  (Side side) const { return m_state[side].increment  ; }
int                 TimeControl::byo_periods(Side side) const { return m_state[side].byo_periods; }

/**
 * Set the main time
 */
void TimeControl::set_main_time(Side side, TimeDuration value)
{
	if(value.is_negative()) {
		THROW_EXCEPTION(RuntimeException(_("Cannot set a negative value as a main time")));
	}
	m_state[side].main_time = std::move(value);
}

/**
 * Set the increment
 */
void TimeControl::set_increment(Side side, TimeDuration value)
{
	if(value.is_negative()) {
		THROW_EXCEPTION(RuntimeException(_("Cannot set a negative value as an increment")));
	}
	m_state[side].increment = std::move(value);
}

/**
 * Set the number of byo-yomi periods
 */
void TimeControl::set_byo_periods(Side side, int value)
{
	if(value<0) {
		THROW_EXCEPTION(RuntimeException(_("Cannot set a negative value as a number of byo-yomi periods")));
	}
	m_state[side].byo_periods = value;
}

/**
 * Check whether both sides have the same time parameters
 */
bool TimeControl::both_sides_have_same_time() const
{
	if(m_state[LEFT].main_time!=m_state[RIGHT].main_time) {
		return false;
	}
	if(m_mode==FISCHER || m_mode==BRONSTEIN) {
		return m_state[LEFT].increment==m_state[RIGHT].increment;
	}
	else if(m_mode==BYO_YOMI) {
		return
			m_state[LEFT].increment  ==m_state[RIGHT].increment   &&
			m_state[LEFT].byo_periods==m_state[RIGHT].byo_periods;
	}
	else {
		return true;
	}
}

/**
 * Name of the current time control mode
 */
const std::string &TimeControl::mode_name() const
{
	return mode_name(m_mode);
}

/**
 * String describing the current time control
 */
std::string TimeControl::description() const
{
	std::stringstream retval;
	retval << *this;
	return retval.str();
}

/**
 * Output in a stream
 */
std::ostream &operator<<(std::ostream &lhs, const TimeControl &rhs)
{
	lhs << rhs.mode_name() << "\t\t";
	rhs.side_description(lhs, LEFT);
	if(!rhs.both_sides_have_same_time()) {
		lhs << " (" << _("left") << ")" << "\t\t";
		rhs.side_description(lhs, RIGHT);
		lhs << " (" << _("right") << ")";
	}
	return lhs;
}

/**
 * Description of the time affected to one of the side
 */
void TimeControl::side_description(std::ostream &stream, Side side) const
{
	format_time(stream, m_state[side].main_time);
	if(m_mode==FISCHER || m_mode==BRONSTEIN) {
		stream << " + ";
		format_time(stream, m_state[side].increment);
		stream << " " << _("by move");
	}
	else if(m_mode==BYO_YOMI) {
		int periods = m_state[side].byo_periods;
		if(periods>=1) {
			stream << " + ";
			format_time(stream, m_state[side].increment);
			stream << " × " << periods << " " << (periods==1 ? _("byo-yomi period") : _("byo-yomi periods"));
		}
	}
}

/**
 * Format a duration given as a number of milliseconds
 */
void TimeControl::format_time(std::ostream &stream, const TimeDuration &value)
{
	// Rounding the number of milliseconds to the closest full second
	int rounded_value = (value.total_milliseconds()+500) / 1000;

	// Special case: no time
	if(rounded_value==0) {
		static const std::string zero_retval = boost::str(boost::format(_("%1% sec")) % 0);
		stream << zero_retval;
		return;
	}

	// Decomposition hh/mm/ss
	int hrs = (rounded_value / (60*60));
	int min = (rounded_value /  60    ) % 60;
	int sec =  rounded_value            % 60;

	// Result
	static boost::format hrs_pattern(_("%1% hour"));
	static boost::format min_pattern(_("%1% min" ));
	static boost::format sec_pattern(_("%1% sec" ));
	bool append_space = false;
	if(hrs!=0) { if(append_space) stream<<" "; append_space=true; hrs_pattern.clear(); stream<<(hrs_pattern%hrs); }
	if(min!=0) { if(append_space) stream<<" "; append_space=true; min_pattern.clear(); stream<<(min_pattern%min); }
	if(sec!=0) { if(append_space) stream<<" "; append_space=true; sec_pattern.clear(); stream<<(sec_pattern%sec); }
}

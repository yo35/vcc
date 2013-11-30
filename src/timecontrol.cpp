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
#include <stdexcept>
#include <sstream>
#include <boost/format.hpp>


// Name of a time control mode.
const std::string &TimeControl::mode_name(Mode mode)
{
	static const Enum::array<Mode, std::string> retval =
	{
		_("Sudden death"),
		_("Fischer"     ),
		_("Bronstein"   ),
		_("Hourglass"   ),
		_("Byo-yomi"    )
	};
	return retval[mode];
}


// Set the main time.
void TimeControl::set_main_time(Side side, TimeDuration value)
{
	if(value < TimeDuration::zero()) {
		throw std::invalid_argument(_("Cannot set a negative value as a main time."));
	}
	_main_time[side] = std::move(value);
}


// Set the increment.
void TimeControl::set_increment(Side side, TimeDuration value)
{
	if(value < TimeDuration::zero()) {
		throw std::invalid_argument(_("Cannot set a negative value as an increment."));
	}
	_increment[side] = std::move(value);
}


// Set the number of byo-yomi periods.
void TimeControl::set_byo_periods(Side side, int value)
{
	if(value<0) {
		throw std::invalid_argument(_("Cannot set a negative value as a number of byo-yomi periods."));
	}
	_byo_periods[side] = value;
}


// Check whether both sides have the same time parameters.
bool TimeControl::both_sides_have_same_time() const
{
	if(_main_time[Side::LEFT]!=_main_time[Side::RIGHT]) {
		return false;
	}
	if(_mode==Mode::FISCHER || _mode==Mode::BRONSTEIN) {
		return _increment[Side::LEFT]==_increment[Side::RIGHT];
	}
	else if(_mode==Mode::BYO_YOMI) {
		return
			_increment  [Side::LEFT]==_increment  [Side::RIGHT] &&
			_byo_periods[Side::LEFT]==_byo_periods[Side::RIGHT];
	}
	else {
		return true;
	}
}


// Name of the current time control mode
const std::string &TimeControl::mode_name() const
{
	return mode_name(_mode);
}


// Human-readable description of the time-control.
std::string TimeControl::description() const
{
	std::ostringstream buffer;
	buffer << mode_name() << "\t\t";
	side_description(buffer, Side::LEFT);
	if(!both_sides_have_same_time()) {
		buffer << " (" << _("left") << ")" << "\t\t";
		side_description(buffer, Side::RIGHT);
		buffer << " (" << _("right") << ")";
	}
	return buffer.str();
}


// Description of the time associated to one of the side.
void TimeControl::side_description(std::ostream &stream, Side side) const
{
	format_time(stream, _main_time[side]);
	if(_mode==Mode::FISCHER || _mode==Mode::BRONSTEIN) {
		stream << " + ";
		format_time(stream, _increment[side]);
		stream << " " << _("by move");
	}
	else if(_mode==Mode::BYO_YOMI) {
		if(_byo_periods[side]>=1) {
			stream << " + ";
			format_time(stream, _increment[side]);
			stream << " × " << _byo_periods[side] << " "
				<< (_byo_periods[side]==1 ? _("byo-yomi period") : _("byo-yomi periods"));
		}
	}
}


// Format a duration given as a number of milliseconds.
void TimeControl::format_time(std::ostream &stream, const TimeDuration &value)
{
	// Rounding the number of milliseconds to the closest full second
	int rounded_value = (value.count()+500) / 1000;

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

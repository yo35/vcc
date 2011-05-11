/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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
#include "strings.h"
#include <translation.h>

// Constantes pour les types de cadences
const TimeControlType SUDDEN_DEATH(0);
const TimeControlType FISCHER     (1);
const TimeControlType BRONSTEIN   (2);
const TimeControlType HOURGLASS   (3);
const TimeControlType BYO_YOMI    (4);

// Constructeur
TimeControl::TimeControl() {
	m_mode = SUDDEN_DEATH;
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_main_time [*k] = 0;
		m_increment [*k] = 0;
		m_byo_period[*k] = 0;
	}
}

// Accesseurs
TimeControlType TimeControl::mode() const {
	return m_mode;
}

int TimeControl::main_time(const Side &side) const {
	return m_main_time[side];
}

int TimeControl::increment(const Side &side) const {
	assert(m_mode==FISCHER || m_mode==BRONSTEIN || m_mode==BYO_YOMI);
	return m_increment[side];
}

int TimeControl::byo_period(const Side &side) const {
	assert(m_mode==BYO_YOMI);
	return m_byo_period[side];
}

// Modifieurs
void TimeControl::set_mode(const TimeControlType &new_mode) {
	m_mode = new_mode;
}

void TimeControl::set_main_times(int new_main_times) {
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_main_time[*k] = new_main_times;
	}
}

void TimeControl::set_main_time(int new_main_time, const Side &side) {
	m_main_time[side] = new_main_time;
}

void TimeControl::set_increments(int new_increments) {
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_increment[*k] = new_increments;
	}
}

void TimeControl::set_increment(int new_increment, const Side &side) {
	m_increment[side] = new_increment;
}

void TimeControl::set_byo_periods(int new_byo_periods) {
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		m_byo_period[*k] = new_byo_periods;
	}
}

void TimeControl::set_byo_period(int new_byo_period, const Side &side) {
	m_byo_period[side] = new_byo_period;
}

bool TimeControl::both_sides_have_same_time() const {
	bool retval = m_main_time[LEFT]==m_main_time[RIGHT];
	if(m_mode==FISCHER || m_mode==BRONSTEIN || m_mode==BYO_YOMI) {
		retval = retval && (m_increment[LEFT]==m_increment[RIGHT]);
	}
	if(m_mode==BYO_YOMI) {
		retval = retval && (m_byo_period[LEFT]==m_byo_period[RIGHT]);
	}
	return retval;
}

std::string time_control_type_name(const TimeControlType &type) {
	static bool do_initialisation = true;
	static EnumArray<TimeControlType, std::string> retval;
	if(do_initialisation) {
		retval[SUDDEN_DEATH] = _("Sudden death");
		retval[FISCHER     ] = _("Fischer"     );
		retval[BRONSTEIN   ] = _("Bronstein"   );
		retval[HOURGLASS   ] = _("Hourglass"   );
		retval[BYO_YOMI    ] = _("Byo-Yomi"    );
		do_initialisation = false;
	}
	return retval[type];
}

std::string format_time(int src) {
	assert(src>=0);

	// Conversion du temps exprimé en millisecondes en un temps en secondes
	int rounded_time = (src+499)/1000;

	// Si le temps restant est de moins d'une heure, on affiche m.ss
	if(rounded_time < 60*60) {
		int sec = rounded_time % 60;
		int min = rounded_time / 60;
		std::string txt_min = int_to_string(min);
		std::string txt_sec = int_to_string(sec);
		if(sec < 10) {
			txt_sec = "0" + txt_sec;
		}
		return txt_min + "." + txt_sec;
	}

	// Si le temps restant est de plus d'une heure, on affiche h:mm
	else {
		rounded_time = rounded_time / 60;
		int min  = rounded_time % 60;
		int hour = rounded_time / 60;
		std::string txt_min  = int_to_string(min );
		std::string txt_hour = int_to_string(hour);
		if(min < 10) {
			txt_min = "0" + txt_min;
		}
		return txt_hour + ":" + txt_min;
	}
}

std::string format_time_long(int src) {
	assert(src>=0);

	// Conversion du temps exprimé en millisecondes en un temps en secondes
	int rounded_time = (src+499)/1000;

	// Cas particulier : durée nulle
	if(rounded_time==0) {
		return std::string("0 ") + _("sec");
	}

	// Extraction des hh/mm/ss
	int sec =  rounded_time            % 60;
	int min = (rounded_time /  60    ) % 60;
	int hr  = (rounded_time / (60*60));

	// Résultat
	std::string res = "";
	if(hr !=0) { if(!res.empty()) res+=" "; res += int_to_string(hr ) + std::string(" ") + _("hour"); }
	if(min!=0) { if(!res.empty()) res+=" "; res += int_to_string(min) + std::string(" ") + _("min" ); }
	if(sec!=0) { if(!res.empty()) res+=" "; res += int_to_string(sec) + std::string(" ") + _("sec" ); }
	return res;
}

std::string TimeControl::description() const {
	std::string res = time_control_type_name(m_mode);
	res += "\t\t" + aux_description(LEFT);
	if(!both_sides_have_same_time()) {
		res += std::string(" (") + _("left" ) + std::string(")");
		res += "\t\t" + aux_description(RIGHT);
		res += std::string(" (") + _("right") + std::string(")");
	}
	return res;
}

std::string TimeControl::aux_description(const Side &side) const {
	std::string res = format_time_long(m_main_time[side]);
	if(m_mode==FISCHER || m_mode==BRONSTEIN) {
		res += " + " + format_time_long(m_increment[side]) + std::string(" ") + _("by move");
	}
	else if(m_mode==BYO_YOMI) {
		res += " + " + format_time_long(m_increment[side]) + std::string(" × ") +
			int_to_string(m_byo_period[side]) + std::string(" ");
		if(m_byo_period[side]<=1)
			res += _("byo-yomi period");
		else
			res += _("byo-yomi periods");
	}
	return res;
}

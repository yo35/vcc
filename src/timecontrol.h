/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
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


#ifndef TIMECONTROL_H
#define TIMECONTROL_H

#include "side.h"

// Cadence
class TimeControl {

public:

	// 4 modes de contrôle de temps sont disponibles
	typedef enum {
		SUDDEN_DEATH,
		FISCHER     ,
		BRONSTEIN   ,
		HOURGLASS
	} Mode;

	// Constructeur
	TimeControl();

	// Accesseurs
	Mode mode() const;
	int  main_time(const Side &side) const;
	int  increment(const Side &side) const;

	// Modifieurs
	void set_mode(Mode new_mode);
	void set_main_times(int new_main_times);
	void set_increments(int new_increments);
	void set_main_time (int new_main_time, const Side &side);
	void set_increment (int new_increment, const Side &side);

	// Test si les deux côtés ont la même config
	bool both_sides_have_same_time() const;

private:

	// Données membres
	Mode         m_mode;
	IntSideArray m_main_time;
	IntSideArray m_increment;
};

#endif

/*
 *    This file is part of Virtual Chess Clock, a chess clock software
 *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TIMECONTROL_H
#define TIMECONTROL_H

class TimeControl {

public:

	// 4 modes de contrôle de temps sont disponibles
	typedef enum {
		SIMPLE_DELAY,
		FISCHER     ,
		BRONSTEIN   ,
		HOUR_GLASS
	} Mode;

	// Constructeur
	TimeControl();

	// Accesseurs
	Mode mode() const;
	int  main_time(int no) const;
	int  increment(int no) const;

	// Modifieurs
	void set_mode(Mode new_mode);
	void set_main_time(int new_main_time, int no=-1);
	void set_increment(int new_increment, int no=-1);

	// Test si les deux côtés ont la même config
	bool both_sides_have_same_time() const;

private:

	// Données membres
	Mode m_mode;
	int  m_main_time[2];
	int  m_increment[2];
};

#endif

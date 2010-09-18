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


#ifndef PARAMS_H
#define PARAMS_H

#include "timecontrol.h"
#include "keys.h"
#include "icon.h"
#include <set>

class Params {
public:

	// Constructeur
	Params(const std::string &prefix_path);

	// Cadence de jeu
	TimeControl time_control;

	// Zones actives pour les touches clavier
	std::set<Keycode> key_area[2];
	void init_kb_areas(const KeyvalList &area_left, const KeyvalList &area_right);

	// Icones
	Icon icon_reset;
	Icon icon_pause;
	Icon icon_tctrl;

	// Pointe vers le répertoire de base du package
	const std::string &prefix_path() const;

private:
	std::set<Keycode> aux_init_kb_areas(const KeyvalList &src);

	std::string m_prefix_path;
};

extern Params *gp;

#endif

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


#ifndef AREAMAP_H
#define AREAMAP_H

#include "side.h"
#include <vector>
#include <string>

class AreaMap {

public:

	// Constructeur
	AreaMap();

	// Accesseurs
	int nb_keys()             const;
	bool is_affected(int key) const;
	Side side       (int key) const;

	// Modifieurs
	void set_nb_keys(int src);
	void set_unaffected(int key);
	void set_side(int key, const Side &src);
	void clear();

	// Fonctions de lecture/écriture dans le fichier sous-jacent
	void load(const std::string &path);
	void save(const std::string &path) const;

private:

	// Données membres
	typedef struct {
		bool affected;
		Side side    ;
	} Node;
	std::vector<Node> m_data;
};

#endif

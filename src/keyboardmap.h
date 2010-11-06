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


#ifndef KEYBOARDMAP_H
#define KEYBOARDMAP_H

#include "keys.h"
#include <string>
#include <fstream>


// Plan de clavier
class KeyboardMap {

public:

	// Constructeur
	KeyboardMap();

	// Accesseurs
	int                      nb_lines     () const;
  int                      default_width() const;
  int                      line_width   () const;
	const PhysicalKeyVector &keys         () const;

	// Fonctions de lecture/écriture dans le fichier sous-jacent
	void load(const std::string &path);
	void save(const std::string &path) const;

private:

	// Sous-routine de la fonction de chargement
	static bool parse_line(std::ifstream &file, const std::string &path,
		int &curr_no_line, std::list<int> &retval);
	static void check_consistency(bool test, const std::string &path, int curr_no_line);

	// Données membres
	int               m_nb_lines     ;
	int               m_default_width;
	int               m_line_width   ;
	PhysicalKeyVector m_keys         ;
};

#endif

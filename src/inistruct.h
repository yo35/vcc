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


#ifndef INISTRUCT_H
#define INISTRUCT_H

#include <string>
#include <map>

class IniStruct {

public:

	// Aliases
	typedef std::string            Key    ;
	typedef std::string            Value  ;
	typedef std::map<Key, Value  > Section;
	typedef std::map<Key, Section> Tree   ;

	// Données
	Tree root;

	// Fonctions de lecture/écriture
	void load(const std::string &path);
	void save(const std::string &path) const;

private:

	// Fonctions auxilliaires
	static std::string trim(const std::string &src);
	static bool is_valid_key(const Key &src);
	static bool is_valid_id_char(char src);
	static bool is_space_char(char src);
};

#endif

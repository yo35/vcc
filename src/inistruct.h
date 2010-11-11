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
#include <set>
#include "enumerable.h"

class IniStruct {

public:

	// Aliases
	typedef std::string            Key    ;
	typedef std::string            Value  ;
	typedef std::map<Key, Value  > Section;
	typedef std::map<Key, Section> Tree   ;

	// Fonctions de lecture/écriture dans le fichier sous-jacent
	void load(const std::string &path);
	void save(const std::string &path) const;

	// Recherche des entrées
	std::set<Key> sections() const;
	std::set<Key> codes(const Key &section) const;

	// Lecture d'une data
	template<class T>
	Enumerable<T> get_data(const Key &section, const Key &code, const Enumerable<T> &default_value) const;
	int           get_data(const Key &section, const Key &code, int                  default_value) const;
	std::string   get_data(const Key &section, const Key &code, const std::string   &default_value) const;

	// Écriture d'une data
	template<class T>
	void set_data(const Key &section, const Key &code, const Enumerable<T> &value);
	void set_data(const Key &section, const Key &code, int                  value);
	void set_data(const Key &section, const Key &code, const std::string   &value);

private:

	// Fonctions auxilliaires
	static bool is_valid_key(const Key &src);
	static bool is_valid_id_char(char src);

	// Données
	Tree root;
};

#include "inistruct.inl"

#endif

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


#ifndef STRINGS_H
#define STRINGS_H

#include <string>
#include <list>

// Supprime les espaces en début et fin de chaîne
std::string trim(const std::string &src);

// Découpe une chaîne de caractères en fonction du séparateur 'sep'
std::list<std::string> split(const std::string &src, char sep);

// Conversion d'un entier en chaîne de caractères
std::string int_to_string(int src);

// Conversion d'une chaîne de caractères en entier
bool is_valid_int(const std::string &src);
int string_to_int(const std::string &src);


#endif

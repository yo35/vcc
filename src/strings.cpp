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


#include "strings.h"
#include <cctype>
#include <cassert>
#include <sstream>


// Supprime les espaces en début et fin de chaîne
std::string trim(const std::string &src) {
	int beg = -1;
	while(true) {
		++beg;
		if(beg>=static_cast<int>(src.length()))
			return "";
		if(!isspace(src.at(beg)))
			break;
	}
	int end = src.length();
	while(true) {
		--end;
		assert(end>=0);
		if(!isspace(src.at(end)))
			break;
	}
	assert(beg <= end);
	return src.substr(beg, end-beg+1);
}

// Découpe une chaîne de caractères en fonction du séparateur 'sep'
std::list<std::string> split(const std::string &src, char sep) {
	std::list<std::string> retval;
	std::string curr_str = "";
	for(unsigned int pos = 0; pos<src.size(); ++pos) {
		if(src.at(pos) == sep) {
			if(curr_str != "") {
				retval.push_back(curr_str);
				curr_str = "";
			}
		}
		else
			curr_str += src.at(pos);
	}
	if(curr_str != "")
		retval.push_back(curr_str);
	return retval;
}

// Conversion d'un entier en chaîne de caractères
std::string int_to_string(int src, bool base16) {
	std::stringstream buff;
	if(base16) buff << std::hex;
	buff << src;
	return buff.str();
}

// Conversion string -> int si possible
bool is_valid_int(const std::string &src) {
	size_t pos0 = 0;
	if(src.empty())
		return false;
	if(src.at(0) == '-') {
		pos0 = 1;
		if(src.length()==1)
			return false;
	}
	for(size_t pos=pos0; pos<src.length(); ++pos) {
		char curr_car = src.at(pos);
		if(!(curr_car>='0' && curr_car<='9'))
			return false;
	}
	return true;
}

// Conversion d'une chaîne de caractères en entier
int string_to_int(const std::string &src) {

	// Initialisation
	size_t pos0 = 0;
	bool is_negative = false;
	int res = 0;
	assert(!src.empty());

	// Entiers négatifs
	if(src.at(0) == '-') {
		is_negative = true;
		pos0 = 1;
		assert(src.length()>1);
	}

	// Calcul de la valeur absolue du résultat
	for(size_t pos=pos0; pos<src.length(); ++pos) {
		char curr_car = src.at(pos);
		assert(curr_car>='0' && curr_car<='9');
		res = res*10 + static_cast<int>(curr_car - '0');
	}

	// Valeure finale
	return is_negative ? -res : res;
}

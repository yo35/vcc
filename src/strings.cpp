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


#include "strings.h"
#include <cctype>
#include <cassert>

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

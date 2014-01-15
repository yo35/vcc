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


#include "shortcutmap.h"
#include <boost/property_tree/xml_parser.hpp>


// Load the shortcut map from a file.
ShortcutMap &ShortcutMap::load(const std::string &path)
{
	boost::property_tree::ptree data;
	boost::property_tree::read_xml(path, data, boost::property_tree::xml_parser::trim_whitespace);
	return load(data.get_child("shortcut-map"));
}


// Load the shortcut map from a property tree.
ShortcutMap &ShortcutMap::load(const boost::property_tree::ptree &data)
{
	_shortcut_low .clear();
	_shortcut_high.clear();

	// Visit each node <key></key>
	for(const auto &it : data) {
		if(it.first!="key") {
			continue;
		}

		// Read the ID of the key.
		std::string id = it.second.get<std::string>("id");

		// Read the shortcut indexes.
		_shortcut_low [id] = it.second.get("low" , 0);
		_shortcut_high[id] = it.second.get("high", 0);
	}

	// Return the object.
	return *this;
}

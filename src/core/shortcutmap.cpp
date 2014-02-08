/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2014 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


// Copy constructor.
ShortcutMap::ShortcutMap(const ShortcutMap &rhs) :
	_shortcut_low (rhs._shortcut_low ),
	_shortcut_high(rhs._shortcut_high)
{}


// Copy operator.
ShortcutMap &ShortcutMap::operator=(const ShortcutMap &rhs)
{
	if(this!=&rhs) {
		_shortcut_low  = rhs._shortcut_low ;
		_shortcut_high = rhs._shortcut_high;
		_signal_changed();
	}
	return *this;
}


// Set the index of the low-position shortcut associated to the key having the ID `id`.
void ShortcutMap::set_shortcut_low(const std::string &id, int value)
{
	int old_value = shortcut_low(id);
	if(old_value!=value) {
		_shortcut_low[id] = value;
		_signal_changed();
	}
}


// Set the index of the high-position shortcut associated to the key having the ID `id`.
void ShortcutMap::set_shortcut_high(const std::string &id, int value)
{
	int old_value = shortcut_high(id);
	if(old_value!=value) {
		_shortcut_high[id] = value;
		_signal_changed();
	}
}


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

	// Notify the modifications and return the object.
	_signal_changed();
	return *this;
}


// Save the shortcut map to a file.
const ShortcutMap &ShortcutMap::save(const std::string &path) const
{
	boost::property_tree::ptree root;
	boost::property_tree::ptree data;
	save(data);
	root.put_child("shortcut-map", data);
	boost::property_tree::xml_writer_settings<boost::property_tree::ptree::key_type::value_type> settings('\t', 1);
	boost::property_tree::write_xml(path, root, std::locale(), settings);
	return *this;
}


// Save the shortcut map to a property tree.
const ShortcutMap &ShortcutMap::save(boost::property_tree::ptree &data) const
{
	// For each key that is binded to a "low-position" shortcut, create a property tree node.
	std::map<std::string, boost::property_tree::ptree> nodes;
	for(const auto &it : _shortcut_low) {
		if(it.second==0) {
			continue;
		}
		nodes[it.first].put("low", it.second);
	}

	// For each key that is binded to a "high-position" shortcut, update the corresponding
	// node, or create new one.
	for(const auto &it : _shortcut_high) {
		if(it.second==0) {
			continue;
		}
		nodes[it.first].put("high", it.second);
	}

	// Insert each node created in the previous steps as a <key></key> node in the property tree.
	for(auto &it : nodes) {
		it.second.put("id", it.first);
		data.add_child("key", it.second);
	}

	// Return a reference to `this`.
	return *this;
}

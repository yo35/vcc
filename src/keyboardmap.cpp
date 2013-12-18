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


#include "keyboardmap.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>


// Constructor of the KeyDescriptor class.
KeyBoardMap::KeyDescriptor::KeyDescriptor() :
	_scan_code_unix   (0),
	_scan_code_windows(0),
	_in_numeric_keypad(false),
	_line_bottom      (0),
	_line_top         (0)
{}


// Load the key-descriptor from a property tree.
KeyBoardMap::KeyDescriptor &KeyBoardMap::KeyDescriptor::load(const boost::property_tree::ptree &data)
{
	// Scan codes
	_scan_code_unix    = data.get<std::uint32_t>("scan-code.unix"   );
	_scan_code_windows = data.get<std::uint32_t>("scan-code.windows");

	// Geometry
	_in_numeric_keypad = data.get<bool>("geometry.in-nkp");
	std::size_t line1 = data.get<std::size_t>("geometry.line-bottom");
	std::size_t line2 = data.get<std::size_t>("geometry.line-top"   );
	_line_bottom = std::max(line1, line2);
	_line_top    = std::min(line1, line2);
	std::size_t line_count = _line_bottom - _line_top + 1;
	_per_line_x     = parse_int_list(data.get<std::string>("geometry.x"    ), line_count,   0);
	_per_line_width = parse_int_list(data.get<std::string>("geometry.width"), line_count, 100);

	// ID & Label
	_id    = data.get<std::string>("id"   );
	_label = data.get<std::string>("label");

	// Return a reference to the object
	return *this;
}


// Constructor.
KeyBoardMap::KeyBoardMap() :
	_total_height           (0),
	_total_width_with_nkp   (0),
	_total_width_without_nkp(0)
{}


// Load the key-board map from a file.
KeyBoardMap &KeyBoardMap::load(const std::string &path)
{
	boost::property_tree::ptree data;
	boost::property_tree::read_xml(path, data, boost::property_tree::xml_parser::trim_whitespace);
	return load(data.get_child("key-board-map"));
}


// Load the key-board map from a property tree.
KeyBoardMap &KeyBoardMap::load(const boost::property_tree::ptree &data)
{
	// Headers
	_id   = data.get<std::string>("id"  );
	_name = data.get<std::string>("name");

	// Key-lines
	std::size_t key_line_count = data.get<std::size_t>("key-line.count");
	_line_height = parse_int_list(data.get<std::string>("key-line.height"), key_line_count, 100);
	_line_y.resize(key_line_count);
	if(key_line_count>0) {
		_line_y[0] = 0;
	}
	for(std::size_t k=1; k<key_line_count; ++k) {
		_line_y[k] = _line_y[k-1] + _line_height[k-1];
	}
	_total_height = key_line_count>0 ? _line_y[key_line_count-1]+_line_height[key_line_count-1] : 0;

	// Keys
	const boost::property_tree::ptree &key_data(data.get_child("keys"));
	_total_width_with_nkp    = 0;
	_total_width_without_nkp = 0;
	for(const auto &it : key_data) {
		if(it.first!="key") {
			continue;
		}

		// Load the key descriptor
		_keys.push_back(KeyDescriptor());
		const KeyDescriptor &key(_keys.back().load(it.second));

		// Update the geometry flags
		for(std::size_t line=key.line_top(); line<=key.line_bottom(); ++line) {
			int x_at_key_right = key.x(line) + key.width(line);
			_total_width_with_nkp = std::max(_total_width_with_nkp, x_at_key_right);
			if(key.in_numeric_keypad()) {
				_total_width_without_nkp = std::max(_total_width_without_nkp, x_at_key_right);
			}
		}
	}

	// Return a reference to the object
	return *this;
}


// Parse a list of comma-separated integers
std::vector<int> KeyBoardMap::parse_int_list(const std::string &data, std::size_t expected_count, int default_value)
{
	// Allocate the returned object
	std::vector<int> retval(expected_count, default_value);

	// Split the string
	std::list<std::string> values;
	boost::split(values, data, boost::is_any_of(","));

	// Try to parse each field of the input string as an integer
	try {
		std::size_t idx = 0;
		for(const auto &it : values) {
			if(idx>=retval.size()) {
				break;
			}
			retval[idx] = boost::lexical_cast<int>(boost::trim_copy(it));
			++idx;
		}
	}
	catch(boost::bad_lexical_cast &) { /* TODO: forward an appropriate exception */ }

	// Return the result
	return std::move(retval);
}

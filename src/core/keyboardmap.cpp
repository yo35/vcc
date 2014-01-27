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


#include "keyboardmap.h"
#include <wrappers/translation.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/xml_parser.hpp>


// Constructor of the KeyDescriptor class.
KeyboardMap::KeyDescriptor::KeyDescriptor() :
	_scan_code_unix   (0),
	_scan_code_windows(0),
	_in_numeric_keypad(false),
	_line_bottom      (0),
	_line_top         (0)
{}


// Load the key-descriptor from a property tree.
KeyboardMap::KeyDescriptor &KeyboardMap::KeyDescriptor::load(const boost::property_tree::ptree &data)
{
	// ID
	_id = data.get<std::string>("id");

	// Label
	std::string rawLabel = data.get<std::string>("label");
	parseLabel(rawLabel);

	// Scan codes
	_scan_code_unix    = data.get<ScanCode>("scan-code.unix"   );
	_scan_code_windows = data.get<ScanCode>("scan-code.windows");

	// Geometry
	_in_numeric_keypad = data.get<bool>("geometry.in-nkp");
	std::size_t line1 = data.get<std::size_t>("geometry.line-bottom");
	std::size_t line2 = data.get<std::size_t>("geometry.line-top"   );
	_line_bottom = std::max(line1, line2);
	_line_top    = std::min(line1, line2);
	std::size_t line_count = _line_bottom - _line_top + 1;
	_per_line_x     = parse_int_list(data.get<std::string>("geometry.x"    ), line_count,   0, 0);
	_per_line_width = parse_int_list(data.get<std::string>("geometry.width"), line_count, 100, MINIMUM_KEY_SIZE);

	// Return a reference to the object
	return *this;
}


// Label parsing method.
void KeyboardMap::KeyDescriptor::parseLabel(const std::string &rawLabel)
{
	static const boost::regex re_special_label("\\{(.+)\\}");
	static const boost::regex re_label_field("(?:^|,)((?:[^,\\\\]|\\\\[,\\\\{}])*)");
	static const boost::regex re_escaped_pattern("\\\\[,\\\\{}]");
	boost::smatch m;
	_label.clear();

	// Special label (for instance: `{caps-lock}`).
	if(boost::regex_match(rawLabel, m, re_special_label) && m.size()==2) {
		_label.push_back(translateSpecialLabel(m[1]));
	}

	// List of labels separated with commas (for instance: `A` or `5,%`).
	else {
		boost::sregex_token_iterator it(rawLabel.begin(), rawLabel.end(), re_label_field, 1), end;
		while(it!=end && _label.size()<MAXIMAL_KEY_LABEL_FIELDS) {
			std::string value = boost::regex_replace(std::string(*it), re_escaped_pattern,
				&KeyboardMap::KeyDescriptor::translateEscapedChar);
			_label.push_back(std::move(value));
			++it;
		}
	}
}


// Translate the escaped character in key labels read from KBM files.
std::string KeyboardMap::KeyDescriptor::translateEscapedChar(const boost::smatch &m)
{
	std::string in = m[0];
	if     (in=="\\\\") return "\\";
	else if(in=="\\," ) return "," ;
	else if(in=="\\{" ) return "{" ;
	else if(in=="\\}" ) return "}" ;
	else return in;
}


// Translate special label IDs into localized strings.
std::string KeyboardMap::KeyDescriptor::translateSpecialLabel(const std::string &code)
{
	static const boost::regex re_f_key("f([0-9]+)");
	boost::smatch m;

	// An empty code is invalid.
	if(code.empty()) return _("Invalid");

	// Match code that is `f` + one or more digit
	else if(boost::regex_match(code, m, re_f_key) && m.size()==2) return "F"+m[1];

	// Main pad keys
	else if(code=="ctrl"       ) return _("Ctrl"      );
	else if(code=="shift"      ) return _("Shift"     );
	else if(code=="alt"        ) return _("Alt"       );
	else if(code=="alt-gr"     ) return _("Alt Gr"    );
	else if(code=="super"      ) return _("Super"     );
	else if(code=="tab"        ) return _("Tab"       );
	else if(code=="caps-lock"  ) return _("Caps\nLock");
	else if(code=="space"      ) return _("Space"     );
	else if(code=="menu"       ) return _("Menu"      );
	else if(code=="back-space" ) return _("Back Space");
	else if(code=="enter"      ) return _("Enter"     );

	// Special function keys
	else if(code=="esc"         ) return _("Esc"          );
	else if(code=="print-screen") return _("Print\nScreen");
	else if(code=="scroll-lock" ) return _("Scroll\nLock" );
	else if(code=="pause"       ) return _("Pause"        );

	// Numeric keypad
	else if(code=="num-lock" ) return _("Num\nLock");
	else if(code=="enter-nkp") return _("Enter"    );

	// Navigation keys
	else if(code=="insert"     ) return _("Ins"       );
	else if(code=="delete"     ) return _("Del"       );
	else if(code=="home"       ) return _("Home"      );
	else if(code=="end"        ) return _("End"       );
	else if(code=="page-up"    ) return _("Page\nUp"  );
	else if(code=="page-down"  ) return _("Page\nDown");
	else if(code=="arrow-up"   ) return "↑";
	else if(code=="arrow-left" ) return "←";
	else if(code=="arrow-down" ) return "↓";
	else if(code=="arrow-right") return "→";

	// Unknown code -> return it "as-is"
	else return code;
}


// Constructor.
KeyboardMap::KeyboardMap() :
	_total_height           (0),
	_total_width_with_nkp   (0),
	_total_width_without_nkp(0)
{}


// Load the keyboard map from a file.
KeyboardMap &KeyboardMap::load(const std::string &path)
{
	boost::property_tree::ptree data;
	boost::property_tree::read_xml(path, data, boost::property_tree::xml_parser::trim_whitespace);
	return load(data.get_child("keyboard-map"));
}


// Load the keyboard map from a property tree.
KeyboardMap &KeyboardMap::load(const boost::property_tree::ptree &data)
{
	// Key-lines
	std::size_t key_line_count = data.get<std::size_t>("key-line.count");
	_line_height = parse_int_list(data.get<std::string>("key-line.height"), key_line_count, 100, MINIMUM_KEY_SIZE);
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
			if(!key.in_numeric_keypad()) {
				_total_width_without_nkp = std::max(_total_width_without_nkp, x_at_key_right);
			}
		}
	}

	// Return a reference to the object
	return *this;
}


// Parse a list of comma-separated integers
std::vector<int> KeyboardMap::parse_int_list(const std::string &data, std::size_t expected_count, int default_value, int min_value)
{
	// Allocate the returned object
	std::vector<int> retval(expected_count, std::max(default_value, min_value));

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
			retval[idx] = std::max(boost::lexical_cast<int>(boost::trim_copy(it)), min_value);
			++idx;
		}
	}
	catch(boost::bad_lexical_cast &) { /* TODO: forward an appropriate exception */ }

	// Return the result
	return std::move(retval);
}


// Return the index of the key-line corresponding to the given y coordinate, if any.
boost::optional<std::size_t> KeyboardMap::line_at(double y) const
{
	for(std::size_t line=0; line<_line_y.size(); ++line) {
		if(y>=_line_y[line] && y<_line_y[line]+_line_height[line]) {
			return line;
		}
	}
	return boost::none;
}


// Return the index of the key corresponding to the given (x,y) coordinates, if any.
boost::optional<std::size_t> KeyboardMap::key_at(double x, double y) const
{
	// Determine the index of the key-line.
	boost::optional<std::size_t> line = line_at(y);
	if(!line) {
		return boost::none;
	}

	// Determine the key.
	for(std::size_t k=0; k<_keys.size(); ++k) {
		const KeyDescriptor &key(_keys[k]);
		if(*line>=key.line_top() && *line<=key.line_bottom() && x>=key.x(*line) && x<key.x(*line)+key.width(*line)) {
			return k;
		}
	}
	return boost::none;
}

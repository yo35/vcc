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


#include "modelmain.h"
#include "modelpaths.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>


// Macro to declare a read-only property.
#define DECLARE_READ_ONLY(property) \
	property( \
		std::bind(&ModelMain::load_ ## property, this, std::placeholders::_1) \
	)


// Macro to declare a read-write property.
#define DECLARE_READ_WRITE(property) \
	property( \
		std::bind(&ModelMain::load_ ## property, this, std::placeholders::_1), \
		std::bind(&ModelMain::save_ ## property, this, std::placeholders::_1) \
	)


// Constructor.
ModelMain::ModelMain() :
	DECLARE_READ_ONLY (config_file       ),
	DECLARE_READ_WRITE(time_control      ),
	DECLARE_READ_WRITE(show_status_bar   ),
	DECLARE_READ_WRITE(reset_confirmation)
{
	register_property(config_file       );
	register_property(time_control      );
	register_property(show_status_bar   );
	register_property(reset_confirmation);

	// Load the file if it exists.
	if(boost::filesystem::exists(config_file())) {
		try {
			boost::property_tree::read_xml(config_file(), _data, boost::property_tree::xml_parser::trim_whitespace);
		}
		catch(boost::property_tree::xml_parser_error &) {
			throw std::runtime_error("An error has occurred while reading the preference file.");
		}
	}

	// Set up the root node.
	if(_data.find("options")==_data.not_found()) {
		_data.put_child("options", ptree());
	}
	_root = &_data.get_child("options");
}


// Save the XML config file.
void ModelMain::finalize_save()
{
	// Create the parent folder if necessary.
	ModelPaths::instance().ensure_config_path_exists();

	// Write the file
	try {
		boost::property_tree::xml_writer_settings<ptree::key_type::value_type> settings('\t', 1);
		boost::property_tree::write_xml(config_file(), _data, std::locale(), settings);
	}
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while writing the preference file.");
	}
}


// Retrieve the node corresponding to the given key. If the node does not exist, it is created.
ModelMain::ptree &ModelMain::fetch(const std::string &key)
{
	if(_root->find(key)==_root->not_found()) {
		_root->put_child(key, ptree());
	}
	return _root->get_child(key);
}


// Convenience function to handle node key that holds a data different for LEFT and RIGHT.
std::string ModelMain::side_key(Side side, const std::string &key)
{
	return (side==Side::LEFT ? "left." : "right.") + key;
}


// Translator class for time durations.
// !!! I do not understand why overloading the operators << and >> is not sufficient
// for TimeDuration to work with ptree. !!!
struct TimeDurationTranslator
{
	typedef std::string  internal_type;
	typedef TimeDuration external_type;
	boost::optional<external_type> get_value(const internal_type &v)
	{
		std::istringstream stream(v);
		external_type retval;
		if(stream >> retval) {
			return retval;
		}
		else {
			return boost::none;
		}
	}
	boost::optional<internal_type> put_value(const external_type &v)
	{
		std::ostringstream buffer;
		buffer<<v;
		return buffer.str();
	}
};
namespace boost { namespace property_tree {
	template<>
	struct translator_between<std::string, TimeDuration>
	{
		typedef TimeDurationTranslator type;
	};
}}



// *****************************************************************************
// Loaders (read-only properties)
// *****************************************************************************


void ModelMain::load_config_file(std::string &target)
{
	target = ModelPaths::instance().config_path() + "/main.xml";
}



// *****************************************************************************
// Loaders & savers (read-write properties)
// *****************************************************************************

void ModelMain::load_time_control(TimeControl &target)
{
	const ptree &node(fetch("time-control"));
	target.set_mode(node.get("mode", TimeControl::Mode::SUDDEN_DEATH));
	for(auto s = Enum::cursor<Side>::first(); s.valid(); ++s) {
		target.set_main_time  (*s, node.get(side_key(*s, "main-time"  ), from_seconds(3*60)));
		target.set_increment  (*s, node.get(side_key(*s, "increment"  ), from_seconds(   2)));
		target.set_byo_periods(*s, node.get(side_key(*s, "byo-periods"), 1));
	}
}


void ModelMain::save_time_control(const TimeControl &value)
{
	ptree &node(fetch("time-control"));
	node.put("mode", value.mode());
	for(auto s = Enum::cursor<Side>::first(); s.valid(); ++s) {
		node.put(side_key(*s, "main-time"  ), value.main_time  (*s));
		node.put(side_key(*s, "increment"  ), value.increment  (*s));
		node.put(side_key(*s, "byo-periods"), value.byo_periods(*s));
	}
}


void ModelMain::load_show_status_bar(bool &target)
{
	target = _root->get("misc-options.show-status-bar", true);
}


void ModelMain::save_show_status_bar(bool value)
{
	_root->put("misc-options.show-status-bar", value);
}


void ModelMain::load_reset_confirmation(ResetConfirmation &target)
{
	target = _root->get("misc-options.reset-confirmation", ResetConfirmation::IF_ACTIVE);
}


void ModelMain::save_reset_confirmation(ResetConfirmation value)
{
	_root->put("misc-options.reset-confirmation", value);
}

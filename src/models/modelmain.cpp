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
#include "modelkeyboard.h"
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
	DECLARE_READ_ONLY (config_file                 ),
	DECLARE_READ_WRITE(time_control                ),
	DECLARE_READ_WRITE(show_status_bar             ),
	DECLARE_READ_WRITE(reset_confirmation          ),
	DECLARE_READ_WRITE(delay_before_display_seconds),
	DECLARE_READ_WRITE(display_time_after_timeout  ),
	DECLARE_READ_WRITE(display_bronstein_extra_info),
	DECLARE_READ_WRITE(display_byo_yomi_extra_info ),
	DECLARE_READ_WRITE(keyboard_id                 ),
	DECLARE_READ_WRITE(keyboard_has_numeric_keypad ),
	DECLARE_READ_WRITE(modifier_keys               ),
	DECLARE_READ_WRITE(left_player                 ),
	DECLARE_READ_WRITE(right_player                ),
	DECLARE_READ_WRITE(show_player_names           )
{
	register_property(config_file                 );
	register_property(time_control                );
	register_property(show_status_bar             );
	register_property(reset_confirmation          );
	register_property(delay_before_display_seconds);
	register_property(display_time_after_timeout  );
	register_property(display_bronstein_extra_info);
	register_property(display_byo_yomi_extra_info );
	register_property(keyboard_id                 );
	register_property(keyboard_has_numeric_keypad );
	register_property(modifier_keys               );
	register_property(left_player                 );
	register_property(right_player                );
	register_property(show_player_names           );

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


void ModelMain::load_delay_before_display_seconds(TimeDuration &target)
{
	target = _root->get("time-options.delay-for-seconds", from_seconds(20*60));
}


void ModelMain::save_delay_before_display_seconds(const TimeDuration &value)
{
	_root->put("time-options.delay-for-seconds", value);
}


void ModelMain::load_display_time_after_timeout(bool &target)
{
	target = _root->get("time-options.time-after-timeout", true);
}


void ModelMain::save_display_time_after_timeout(bool value)
{
	_root->put("time-options.time-after-timeout", value);
}


void ModelMain::load_display_bronstein_extra_info(bool &target)
{
	target = _root->get("time-options.bronstein-extra-info", true);
}


void ModelMain::save_display_bronstein_extra_info(bool value)
{
	_root->put("time-options.bronstein-extra-info", value);
}


void ModelMain::load_display_byo_yomi_extra_info(bool &target)
{
	target = _root->get("time-options.byo-yomi-extra-info", true);
}


void ModelMain::save_display_byo_yomi_extra_info(bool value)
{
	_root->put("time-options.byo-yomi-extra-info", value);
}


void ModelMain::load_keyboard_id(std::string &target)
{
	target = _root->get("keyboard.id", ModelKeyboard::instance().default_id());
}


void ModelMain::save_keyboard_id(const std::string &value)
{
	_root->put("keyboard.id", value);
}


void ModelMain::load_keyboard_has_numeric_keypad(bool &target)
{
	target = _root->get("keyboard.numeric-keypad", true);
}


void ModelMain::save_keyboard_has_numeric_keypad(bool value)
{
	_root->put("keyboard.numeric-keypad", value);
}


void ModelMain::load_modifier_keys(ModifierKeys &target)
{
	target = _root->get("keyboard.modifier-keys", ModifierKeys::DOUBLE_SHIFT);
}


void ModelMain::save_modifier_keys(ModifierKeys value)
{
	_root->put("keyboard.modifier-keys", value);
}


void ModelMain::load_left_player(QString &target)
{
	target = QString::fromStdString(_root->get("players.left", std::string("")));
}


void ModelMain::save_left_player(const QString &value)
{
	_root->put("players.left", value.toStdString());
}


void ModelMain::load_right_player(QString &target)
{
	target = QString::fromStdString(_root->get("players.right", std::string("")));
}


void ModelMain::save_right_player(const QString &value)
{
	_root->put("players.right", value.toStdString());
}


void ModelMain::load_show_player_names(bool &target)
{
	target = _root->get("players.show-names", false);
}


void ModelMain::save_show_player_names(bool value)
{
	_root->put("players.show-names", value);
}

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


#include "params.h"
#include <config.h>
#include <stdexcept>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <QApplication>
#ifdef OS_IS_WINDOWS
	#include <windows.h>
#endif



// Singleton object.
std::unique_ptr<Params> Params::_instance;


// Root path indicating where the application is installed (read-only directory).
const std::string &Params::prefix_path()
{
	if(!_prefix_path) {
		#ifdef OS_IS_WINDOWS
			char buff[2048];
			if(GetModuleFileName(NULL, buff, 2048)==0) {
				throw std::runtime_error("Unable to retrieve the filename of the executable.");
			}
			boost::filesystem::path exe_path(buff);
			_prefix_path = exe_path.parent_path().string() + "/" RPATH_BIN_BACKWARD;
		#else
			_prefix_path = PATH_TOP;
		#endif
	}
	return *_prefix_path;
}


// Configuration folder in the user's home (read-write directory).
const std::string &Params::config_path()
{
	if(!_config_path) {
		#ifdef DEV_COMPILATION
			_config_path = PATH_TOP "/user_config";
		#else
			#ifdef OS_IS_WINDOWS
				std::string config_path = Glib::get_user_config_dir() + "/" PROJECT_NAME;
			#else
				std::string config_path = Glib::get_home_dir() + "/." PROJECT_NAME;
			#endif
			_config_path =  wxStandardPaths::Get().GetUserDataDir();
		#endif
	}
	return *_config_path;
}


// Directory holding data of the application (read-only directory).
const std::string &Params::share_path()
{
	if(!_share_path) {
		_share_path = prefix_path() + "/" RPATH_SHARE;
	}
	return *_share_path;
}


// Current locale.
const std::string &Params::locale()
{
	if(!_locale) {
		_locale = QApplication::inputMethod()->locale().name().toStdString();
	}
	return *_locale;
}


// File that holds the preferences of the current user.
const std::string &Params::config_file()
{
	if(!_config_file) {
		_config_file = config_path() + "/" + _app_short_name + ".xml";
	}
	return *_config_file;
}


// File that contains the index of all available keyboard maps.
const std::string &Params::keyboard_index_file()
{
	if(!_keyboard_index_file) {
		_keyboard_index_file = share_path() + "/keyboards.xml";
	}
	return *_keyboard_index_file;
}


// File that contains the default shortcut map.
const std::string &Params::default_shortcut_map_file()
{
	if(!_default_shortcut_map_file) {
		_default_shortcut_map_file = share_path() + "/default-shortcut-map.xml";
	}
	return *_default_shortcut_map_file;
}


// File that contains the user-defined shortcut map.
const std::string &Params::custom_shortcut_map_file()
{
	if(!_custom_shortcut_map_file) {
		_custom_shortcut_map_file = config_path() + "/shortcut-map.xml";
	}
	return *_custom_shortcut_map_file;
}


// Private constructor.
Params::Params() :
	_app_short_name(APP_SHORT_NAME), _app_name(APP_NAME), _app_full_name(APP_FULL_NAME),
	_app_version(APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH),
	_root(nullptr), _ptree_loaded(false), _ptree_saved(true),
	_keyboard_index_loaded(false), _shortcut_map_loaded(false)
{}


// Load the persistent file that holds the preferences defined by the current user,
// if it is not loaded yet.
void Params::load()
{
	// Nothing to do if the file has already been loaded.
	if(_ptree_loaded) {
		return;
	}

	// Load the file if it exists.
	if(boost::filesystem::exists(config_file())) {
		try {
			boost::property_tree::read_xml(config_file(), _ptree, boost::property_tree::xml_parser::trim_whitespace);
		}
		catch(boost::property_tree::xml_parser_error &) {
			throw std::runtime_error("An error has occurred while reading the preference file.");
		}
	}

	// Set up the root node.
	if(_ptree.find("options")==_ptree.not_found()) {
		_ptree.put_child("options", ptree());
	}
	_root = &_ptree.get_child("options");

	// Set the loaded flag to true.
	_ptree_loaded = true;
}


// Save the preferences defined by the current user.
void Params::save()
{
	// Save the shortcut map file if it is loaded.
	if(_shortcut_map_loaded) {
		try {
			ensure_config_path_exists();
			_shortcut_map.save(custom_shortcut_map_file());
		}
		catch(boost::property_tree::xml_parser_error &) {
			throw std::runtime_error("An error has occurred while writing the shortcut map file.");
		}
	}

	// Nothing else to do if the file has already been saved.
	if(_ptree_saved) {
		return;
	}

	// Write the file
	ensure_config_path_exists();
	try {
		boost::property_tree::xml_writer_settings<ptree::key_type::value_type> settings('\t', 1);
		boost::property_tree::write_xml(config_file(), _ptree, std::locale(), settings);
	}
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while writing the preference file.");
	}

	// Set the saved flag to true
	_ptree_saved = true;
}


// Create the application folder is the user's home if it does not exist yet.
void Params::ensure_config_path_exists()
{
	boost::filesystem::create_directories(config_path());
}


// Return the node corresponding to the given key (the node is created it if it does not exist yet).
Params::ptree &Params::fetch(const std::string &key)
{
	if(_root->find(key)==_root->not_found()) {
		_root->put_child(key, ptree());
	}
	return _root->get_child(key);
}


// Return an atomic-valued option.
template<class T>
T Params::get_atomic_value(const std::string &key, T default_value)
{
	load();
	return _root->get(key, default_value);
}


// Set an atomic-valued option.
template<class T>
void Params::put_atomic_value(const std::string &key, T value)
{
	load();
	_root->put(key, value);
	_ptree_saved = false;
}


// Convenience function to handle node key that holds a data different for LEFT and RIGHT.
std::string Params::side_key(Side side, const std::string &key)
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


// Retrieve the current time control.
TimeControl Params::time_control()
{
	load();
	const ptree &tc_node(fetch("time-control"));
	TimeControl retval;
	retval.set_mode(tc_node.get("mode", TimeControl::Mode::SUDDEN_DEATH));
	for(auto s = Enum::cursor<Side>::first(); s.valid(); ++s) {
		retval.set_main_time  (*s, tc_node.get(side_key(*s, "main-time"  ), from_seconds(3*60)));
		retval.set_increment  (*s, tc_node.get(side_key(*s, "increment"  ), from_seconds(   2)));
		retval.set_byo_periods(*s, tc_node.get(side_key(*s, "byo-periods"), 1));
	}
	return std::move(retval);
}


// Set the persistent time control.
void Params::set_time_control(const TimeControl &value)
{
	load();
	ptree &tc_node(fetch("time-control"));
	tc_node.put("mode", value.mode());
	for(auto s = Enum::cursor<Side>::first(); s.valid(); ++s) {
		tc_node.put(side_key(*s, "main-time"  ), value.main_time  (*s));
		tc_node.put(side_key(*s, "increment"  ), value.increment  (*s));
		tc_node.put(side_key(*s, "byo-periods"), value.byo_periods(*s));
	}
	_ptree_saved = false;
}


// Players' names.
std::string Params::player_name(Side side)
{
	return get_atomic_value(side==Side::LEFT ? "players.left" : "players.right", std::string());
}


// Set the players' names.
void Params::set_player_name(Side side, const std::string &value)
{
	put_atomic_value(side==Side::LEFT ? "players.left" : "players.right", value);
}


// Whether the players' names should be shown or not.
bool Params::show_names()
{
	return get_atomic_value("players.show-names", false);
}


// Set whether the players' names should be shown or not.
void Params::set_show_names(bool value)
{
	put_atomic_value("players.show-names", value);
}


// Whether the status bar should be shown or not.
bool Params::show_status_bar()
{
	return get_atomic_value("misc-options.show-status-bar", true);
}


// Set whether the status bar should be shown or not.
void Params::set_show_status_bar(bool value)
{
	put_atomic_value("misc-options.show-status-bar", value);
}


// Reset confirmation option.
ResetConfirmation Params::reset_confirmation()
{
	return get_atomic_value("misc-options.reset-confirmation", ResetConfirmation::IF_ACTIVE);
}


// Set the reset confirmation option.
void Params::set_reset_confirmation(ResetConfirmation value)
{
	put_atomic_value("misc-options.reset-confirmation", value);
}


// Minimal remaining time before seconds is displayed.
TimeDuration Params::delay_before_display_seconds()
{
	return get_atomic_value("time-options.delay-for-seconds", from_seconds(20*60));
}


// Set the minimal remaining time before seconds are displayed.
void Params::set_delay_before_display_seconds(const TimeDuration &value)
{
	put_atomic_value("time-options.delay-for-seconds", value);
}


// Whether the time should be displayed after timeout.
bool Params::display_time_after_timeout()
{
	return get_atomic_value("time-options.time-after-timeout", true);
}


// Set whether the time should be displayed after timeout.
void Params::set_display_time_after_timeout(bool value)
{
	put_atomic_value("time-options.time-after-timeout", value);
}


// Whether extra-information is displayed in Bronstein-mode.
bool Params::display_bronstein_extra_info()
{
	return get_atomic_value("time-options.bronstein-extra-info", true);
}


// Set whether extra-information is displayed in Bronstein-mode.
void Params::set_display_bronstein_extra_info(bool value)
{
	put_atomic_value("time-options.bronstein-extra-info", value);
}


// Whether extra-information is displayed in byo-yomi-mode.
bool Params::display_byo_yomi_extra_info()
{
	return get_atomic_value("time-options.byo-yomi-extra-info", true);
}


// Set whether extra-information is displayed in byo-yomi-mode.
void Params::set_display_byo_yomi_extra_info(bool value)
{
	put_atomic_value("time-options.byo-yomi-extra-info", value);
}


// Load the keyboard index file if not done yet.
void Params::ensure_keyboard_index_loaded()
{
	if(_keyboard_index_loaded) {
		return;
	}

	try
	{
		// Read the keyboard index file
		ptree keyboard_index;
		boost::property_tree::read_xml(keyboard_index_file(), keyboard_index, boost::property_tree::xml_parser::trim_whitespace);

		// Iterates over the list of keyboards
		const ptree &keyboards(keyboard_index.get_child("keyboards"));
		for(const auto &it : keyboards) {
			if(it.first!="keyboard") {
				continue;
			}
			load_keyboard(it.second);
		}
	}

	// The keyboard index file must be readable.
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while reading the keyboard index file.");
	}

	// Mark the keyboard index file as read.
	_keyboard_index_loaded = true;
}


// Load the keyboard information contained in the given node.
void Params::load_keyboard(const ptree &keyboard)
{
	// Read the ID/name/icon data associated to the given keyboard.
	std::string id = keyboard.get<std::string>("id");
	_keyboard_names[id] = keyboard.get<std::string>("name");
	_keyboard_icons[id] = QIcon(QString::fromStdString(share_path() + "/" + keyboard.get<std::string>("icon")));

	// List the locales for which the keyboard will be considered as the default one.
	for(const auto &it : keyboard.get_child("locales")) {
		if(it.first!="locale") {
			continue;
		}
		std::string locale = it.second.get_value<std::string>();
		if(locale=="*") {
			_default_keyboard = id;
		}
		else {
			_locale_to_keyboard[locale] = id;
		}
	}

	// Register the keyboard in the list of available keyboards.
	_keyboard_list.insert(std::move(id));
}


// Load the shortcut map file if not done yet.
void Params::ensure_shortcut_map_loaded()
{
	if(_shortcut_map_loaded) {
		return;
	}

	try
	{
		// Try to load the user-defined shortcut map file if it exists.
		if(boost::filesystem::exists(custom_shortcut_map_file())) {
			_shortcut_map.load(custom_shortcut_map_file());
		}

		// Otherwise, try to load the default shortcut map file.
		else {
			_shortcut_map.load(default_shortcut_map_file());
		}
	}

	// The keyboard index file must be readable.
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while reading the shortcut map file.");
	}

	// Mark the shortcut map file as read.
	_shortcut_map_loaded = true;
}


// Ensure that the keyboard corresponding to the given ID exists and is actually
// registered in the keyboard index file.
void Params::ensure_keyboard_id_exists(const std::string &id)
{
	ensure_keyboard_index_loaded();
	if(_keyboard_list.count(id)==0) {
		throw std::invalid_argument("Invalid keyboard ID.");
	}
}


// Return the ID of the keyboard that is associated to the given locale.
const std::string &Params::default_keyboard(const std::string &locale)
{
	ensure_keyboard_index_loaded();
	auto it = _locale_to_keyboard.find(locale);
	return it==_locale_to_keyboard.end() ? _default_keyboard : it->second;
}


// Return the ID of the current selected keyboard.
std::string Params::current_keyboard()
{
	return get_atomic_value("keyboard.id", default_keyboard(locale()));
}


// Change the current selected keyboard.
void Params::set_current_keyboard(const std::string &value)
{
	put_atomic_value("keyboard.id", value);
}


// Whether the numeric keypad should be displayed with the keyboard map or not.
bool Params::has_numeric_keypad()
{
	return get_atomic_value("keyboard.numeric-keypad", true);
}


// Whether the numeric keypad should be displayed with the keyboard map or not.
void Params::set_has_numeric_keypad(bool value)
{
	put_atomic_value("keyboard.numeric-keypad", value);
}


// Modifier keys.
ModifierKeys Params::modifier_keys()
{
	return get_atomic_value("keyboard.modifier-keys", ModifierKeys::DOUBLE_SHIFT);
}


// Set the modifier keys.
void Params::set_modifier_keys(ModifierKeys value)
{
	put_atomic_value("keyboard.modifier-keys", value);
}


// Return the keyboard map corresponding to the given ID.
const KeyboardMap &Params::keyboard_map(const std::string &id)
{
	ensure_keyboard_id_exists(id);
	auto it = _keyboard_maps.find(id);
	if(it==_keyboard_maps.end()) {
		try {
			return _keyboard_maps[id].load(share_path() + "/" + id + ".kbm");
		}
		catch(boost::property_tree::ptree_error &) {
			throw std::runtime_error("An error has occurred while reading a keyboard map file.");
		}
	}
	return it->second;
}


// Return the current shortcut map.
ShortcutMap &Params::shortcut_map()
{
	ensure_shortcut_map_loaded();
	return _shortcut_map;
}

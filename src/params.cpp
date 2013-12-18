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
#include <stdexcept>
#include <sstream>
#include <config.h>
#include <translation.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
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
				throw std::runtime_error(_("Unable to retrieve the filename of the executable."));
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


// Name of the file that holds the preferences of the current user.
const std::string &Params::ptree_filename()
{
	if(!_ptree_filename) {
		_ptree_filename = config_path() + "/" + _app_short_name + ".xml";
	}
	return *_ptree_filename;
}


// Private constructor.
Params::Params() :
	_app_short_name(APP_SHORT_NAME), _app_name(APP_NAME), _app_full_name(APP_FULL_NAME),
	_root(nullptr), _ptree_loaded(false), _ptree_saved(true),
	_keyboard_map_loaded(false)
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
	if(boost::filesystem::exists(ptree_filename())) {
		try {
			boost::property_tree::read_xml(ptree_filename(), _ptree, boost::property_tree::xml_parser::trim_whitespace);
		}
		catch(boost::property_tree::xml_parser_error &) {
			throw std::runtime_error(_("An error has occurred while reading the preference file."));
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
	// Nothing to do if the file has already been saved.
	if(_ptree_saved) {
		return;
	}

	// Write the file
	ensure_config_path_exists();
	try {
		boost::property_tree::xml_writer_settings<ptree::key_type::value_type> settings('\t', 1);
		boost::property_tree::write_xml(ptree_filename(), _ptree, std::locale(), settings);
	}
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error(_("An error has occurred while writing the preference file."));
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
		retval.set_main_time  (*s, tc_node.get(side_key(*s, "main-time"  ), TimeDuration(3*60*1000)));
		retval.set_increment  (*s, tc_node.get(side_key(*s, "increment"  ), TimeDuration(   2*1000)));
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


// Reset confirmation option.
ResetConfirmation Params::reset_confirmation()
{
	return get_atomic_value("reset-confirmation", ResetConfirmation::IF_ACTIVE);
}


// Set the reset confirmation option.
void Params::set_reset_confirmation(ResetConfirmation value)
{
	put_atomic_value("reset-confirmation", value);
}


// Whether the status bar should be shown or not.
bool Params::show_status_bar()
{
	return get_atomic_value("display.status-bar", true);
}


// Set whether the status bar should be shown or not.
void Params::set_show_status_bar(bool value)
{
	put_atomic_value("display.status-bar", value);
}


// Load the keyboard map files if not done yet.
void Params::ensure_keyboard_maps_loaded()
{
	if(_keyboard_map_loaded) {
		return;
	}
	boost::filesystem::directory_iterator end;
	for(boost::filesystem::directory_iterator it(share_path()); it!=end; ++it) {
		if(it->path().extension()!=".kbm") {
			continue;
		}

		// Try to load the current file
		try {
			KeyboardMap kbm;
			kbm.load(it->path().c_str());
			_keyboard_maps.insert(kbm.id());
			_keyboard_map_data[kbm.id()] = std::move(kbm);
		}
		catch(boost::property_tree::ptree_error &) {}
	}
	_keyboard_map_loaded = true;
}


// Return the IDs of the available keyboard maps.
const std::set<std::string> &Params::keyboard_maps()
{
	ensure_keyboard_maps_loaded();
	return _keyboard_maps;
}


// Return the keyboard map corresponding to the given ID.
const KeyboardMap &Params::keyboard_map(const std::string &id)
{
	ensure_keyboard_maps_loaded();
	auto it = _keyboard_map_data.find(id);
	if(it==_keyboard_map_data.end()) {
		throw std::invalid_argument(_("Invalid keyboard map ID."));
	}
	else {
		return it->second;
	}
}

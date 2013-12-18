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


#ifndef PARAMS_H_
#define PARAMS_H_

#include <string>
#include <memory>
#include <map>
#include <set>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include "timecontrol.h"
#include "options.h"
#include "keyboardmap.h"


/**
 * Hold all the persistent parameters of the application. This is a singleton class.
 */
class Params
{
public:

	/**
	 * Return the singleton object.
	 */
	static Params &get() { if(!_instance) { _instance.reset(new Params); } return *_instance; }

	/**
	 * Save the singleton object.
	 */
	static void force_save() { get().save(); }

	/**
	 * @name Neither copy nor move is allowed.
	 * @{
	 */
	Params(const Params &op) = delete;
	Params(Params &&op) = delete;
	Params &operator=(const Params &op) = delete;
	Params &operator=(Params &&op) = delete;
	/**@} */

	/**
	 * @name Application directories and files.
	 * @{
	 */
	const std::string &prefix_path   (); //!< Root path indicating where the application is installed (read-only directory).
	const std::string &config_path   (); //!< Configuration folder in the user's home (read-write directory).
	const std::string &share_path    (); //!< Directory holding data of the application (read-only directory).
	const std::string &ptree_filename(); //!< Name of the file that holds the preferences of the current user.
	/**@} */

	/**
	 * @name Application-level read-only properties.
	 * @{
	 */
	const std::string &app_short_name() { return _app_short_name; } //!< Application short name.
	const std::string &app_name      () { return _app_name      ; } //!< Application name (without spaces).
	const std::string &app_full_name () { return _app_full_name ; } //!< Application full name.
	/**@} */

	/**
	 * Retrieve the current time control.
	 */
	TimeControl time_control();

	/**
	 * Set the persistent time control.
	 */
	void set_time_control(const TimeControl &value);

	/**
	 * Reset confirmation option.
	 */
	ResetConfirmation reset_confirmation();

	/**
	 * Set the reset confirmation option.
	 */
	void set_reset_confirmation(ResetConfirmation value);

	/**
	 * Whether the status bar should be shown or not.
	 */
	bool show_status_bar();

	/**
	 * Set whether the status bar should be shown or not.
	 */
	void set_show_status_bar(bool value);

	/**
	 * Wether the numeric keypad should be displayed with the keyboard map or not.
	 */
	bool show_numeric_keypad();

	/**
	 * Wether the numeric keypad should be displayed with the keyboard map or not.
	 */
	void set_show_numeric_keypad(bool value);

	/**
	 * Return the IDs of the available keyboard maps.
	 */
	const std::set<std::string> &keyboard_maps();

	/**
	 * Return the keyboard map corresponding to the given ID.
	 */
	const KeyboardMap &keyboard_map(const std::string &id);

private:

	// Useful alias
	typedef boost::property_tree::ptree ptree;

	// Private constructor
	Params();

	// Load/save functions for the user-related data.
	void load();
	void save();
	void ensure_config_path_exists();
	void ensure_keyboard_maps_loaded();

	// Utility functions to manage the property tree holding the user-related data.
	ptree &fetch(const std::string &key);
	template<class T> T get_atomic_value(const std::string &key, T default_value);
	template<class T> void put_atomic_value(const std::string &key, T value);
	static std::string side_key(Side side, const std::string &key);

	// Application directories and files
	boost::optional<std::string> _prefix_path   ;
	boost::optional<std::string> _config_path   ;
	boost::optional<std::string> _share_path    ;
	boost::optional<std::string> _ptree_filename;

	// General application properties
	std::string _app_short_name;
	std::string _app_name      ;
	std::string _app_full_name ;

	// Property tree for the user-related parameters
	ptree  _ptree       ;
	ptree *_root        ;
	bool   _ptree_loaded;
	bool   _ptree_saved ;

	// Keyboard maps
	bool                               _keyboard_map_loaded;
	std::map<std::string, KeyboardMap> _keyboard_map_data  ;
	std::set<std::string>              _keyboard_maps      ;

	// Singleton object
	static std::unique_ptr<Params> _instance;
};

#endif /* PARAMS_H_ */

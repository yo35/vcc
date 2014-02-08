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
#include <QIcon>
#include <core/options.h>
#include <core/side.h>
#include <core/timecontrol.h>
#include <core/keyboardmap.h>
#include <core/shortcutmap.h>
#include <core/versionnumber.h>


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
	 * @name Application directories, files, and environment.
	 * @{
	 */
	const std::string &config_file              (); //!< File that holds the preferences of the current user.
	const std::string &default_shortcut_map_file(); //!< Path to the default shortcut map file.
	const std::string &custom_shortcut_map_file (); //!< Path to the user-specific shortcut map file.

	/**@} */

	/**
	 * Players' names.
	 */
	std::string player_name(Side side);

	/**
	 * Set the players' names.
	 */
	void set_player_name(Side side, const std::string &value);

	/**
	 * Whether the players' names should be shown or not.
	 */
	bool show_names();

	/**
	 * Set whether the players' names should be shown or not.
	 */
	void set_show_names(bool value);

	/**
	 * Minimal remaining time before seconds is displayed.
	 */
	TimeDuration delay_before_display_seconds();

	/**
	 * Set the minimal remaining time before seconds are displayed.
	 */
	void set_delay_before_display_seconds(const TimeDuration &value);

	/**
	 * Whether the time should be displayed after timeout.
	 */
	bool display_time_after_timeout();

	/**
	 * Set whether the time should be displayed after timeout.
	 */
	void set_display_time_after_timeout(bool value);

	/**
	 * Whether extra-information is displayed in Bronstein-mode.
	 */
	bool display_bronstein_extra_info();

	/**
	 * Set whether extra-information is displayed in Bronstein-mode.
	 */
	void set_display_bronstein_extra_info(bool value);

	/**
	 * Whether extra-information is displayed in byo-yomi-mode.
	 */
	bool display_byo_yomi_extra_info();

	/**
	 * Set whether extra-information is displayed in byo-yomi-mode.
	 */
	void set_display_byo_yomi_extra_info(bool value);

	/**
	 * Return the ID of the current selected keyboard.
	 */
	std::string current_keyboard();

	/**
	 * Change the current selected keyboard.
	 */
	void set_current_keyboard(const std::string &value);

	/**
	 * Whether the numeric keypad should be displayed with the keyboard map or not.
	 */
	bool has_numeric_keypad();

	/**
	 * Whether the numeric keypad should be displayed with the keyboard map or not.
	 */
	void set_has_numeric_keypad(bool value);

	/**
	 * Modifier keys.
	 */
	ModifierKeys modifier_keys();

	/**
	 * Set the modifier keys.
	 */
	void set_modifier_keys(ModifierKeys value);

	/**
	 * Return the current shortcut map.
	 */
	ShortcutMap &shortcut_map();

private:

	// Useful alias
	typedef boost::property_tree::ptree ptree;

	// Private constructor
	Params();

	// Load/save functions for the user-related data.
	void load();
	void save();
	void ensure_config_path_exists();
	void ensure_shortcut_map_loaded();

	// Utility functions to manage the property tree holding the user-related data.
	ptree &fetch(const std::string &key);
	template<class T> T get_atomic_value(const std::string &key, T default_value);
	template<class T> void put_atomic_value(const std::string &key, T value);
	static std::string side_key(Side side, const std::string &key);

	// Application directories, files, and environment.
	boost::optional<std::string> _config_file              ;
	boost::optional<std::string> _default_shortcut_map_file;
	boost::optional<std::string> _custom_shortcut_map_file ;

	// Property tree for the user-related parameters
	ptree  _ptree       ;
	ptree *_root        ;
	bool   _ptree_loaded;
	bool   _ptree_saved ;

	// Keyboard maps
	bool                               _shortcut_map_loaded  ;
	ShortcutMap                        _shortcut_map         ;

	// Singleton object
	static std::unique_ptr<Params> _instance;
};

#endif /* PARAMS_H_ */

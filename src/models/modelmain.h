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


#ifndef MODELMAIN_H_
#define MODELMAIN_H_

#include "abstractmodel.h"
#include <core/singleton.h>
#include <core/timecontrol.h>
#include <core/options.h>
#include <core/keys.h>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <QString>


/**
 * Model holding the general preferences of the user.
 */
class ModelMain : public AbstractModel, public Singleton<ModelMain>
{
	friend class Singleton<ModelMain>;

public:

	/**
	 * Path to the file that holds the preferences of the current user.
	 */
	ReadOnlyProperty<std::string> config_file;

	/**
	 * Time control used by the clock.
	 */
	ReadWriteProperty<TimeControl> time_control;

	/**
	 * Whether the status bar should be shown or not.
	 */
	ReadWriteProperty<bool> show_status_bar;

	/**
	 * Reset confirmation option.
	 */
	ReadWriteProperty<ResetConfirmation> reset_confirmation;

	/**
	 * Minimal remaining time before seconds is displayed.
	 */
	ReadWriteProperty<TimeDuration> delay_before_display_seconds;

	/**
	 * Whether the time should be displayed after timeout.
	 */
	ReadWriteProperty<bool> display_time_after_timeout;

	/**
	 * Whether extra-information is displayed in Bronstein-mode.
	 */
	ReadWriteProperty<bool> display_bronstein_extra_info;

	/**
	 * Whether extra-information is displayed in byo-yomi-mode.
	 */
	ReadWriteProperty<bool> display_byo_yomi_extra_info;

	/**
	 * ID of the current selected keyboard.
	 */
	ReadWriteProperty<std::string> keyboard_id;

	/**
	 * Whether the numeric keypad should be displayed with the keyboard map or not.
	 */
	ReadWriteProperty<bool> keyboard_has_numeric_keypad;

	/**
	 * Modifier keys.
	 */
	ReadWriteProperty<ModifierKeys> modifier_keys;

	/**
	 * Left player's name.
	 */
	ReadWriteProperty<QString> left_player;

	/**
	 * Right player's name.
	 */
	ReadWriteProperty<QString> right_player;

	/**
	 * Whether the players' names should be shown or not.
	 */
	ReadWriteProperty<bool> show_player_names;

protected:

	// Implement the save method.
	void finalize_save() override;

private:

	// Constructor.
	ModelMain();

	// Loaders.
	void load_config_file                 (std::string       &target);
	void load_time_control                (TimeControl       &target);
	void load_show_status_bar             (bool              &target);
	void load_reset_confirmation          (ResetConfirmation &target);
	void load_delay_before_display_seconds(TimeDuration      &target);
	void load_display_time_after_timeout  (bool              &target);
	void load_display_bronstein_extra_info(bool              &target);
	void load_display_byo_yomi_extra_info (bool              &target);
	void load_keyboard_id                 (std::string       &target);
	void load_keyboard_has_numeric_keypad (bool              &target);
	void load_modifier_keys               (ModifierKeys      &target);
	void load_left_player                 (QString           &target);
	void load_right_player                (QString           &target);
	void load_show_player_names           (bool              &target);

	// Savers
	void save_time_control                (const TimeControl  &value);
	void save_show_status_bar             (bool                value);
	void save_reset_confirmation          (ResetConfirmation   value);
	void save_delay_before_display_seconds(const TimeDuration &value);
	void save_display_time_after_timeout  (bool                value);
	void save_display_bronstein_extra_info(bool                value);
	void save_display_byo_yomi_extra_info (bool                value);
	void save_keyboard_id                 (const std::string  &value);
	void save_keyboard_has_numeric_keypad (bool                value);
	void save_modifier_keys               (ModifierKeys        value);
	void save_left_player                 (const QString      &value);
	void save_right_player                (const QString      &value);
	void save_show_player_names           (bool                value);

	// Useful alias
	typedef boost::property_tree::ptree ptree;

	// Private methods
	ptree &fetch(const std::string &key);
	static std::string side_key(Side side, const std::string &key);

	// Private members
	ptree  _data;
	ptree *_root;
};

#endif /* MODELMAIN_H_ */

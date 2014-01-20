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


#ifndef SHORTCUTMANAGER_H_
#define SHORTCUTMANAGER_H_

#include "keys.h"
#include "side.h"
#include "keyboardmap.h"
#include "shortcutmap.h"
#include <map>


/**
 * For each key (identified by its low-level scan-code), this object associate
 * the index of the shortcuts that are triggered when it is pressed.
 */
class ShortcutManager
{
public:

	/**
	 * Constructor.
	 */
	ShortcutManager();

	/**
	 * Return the scan-code corresponding to the modifier key on the given side.
	 */
	ScanCode modifier_key(Side side) const { return _modifier_key[side]; }

	/**
	 * Return the index of the low-position shortcut associated to the key corresponding
	 * to the given scan-code. If the scan-code does not refer to a key, or if no
	 * shortcut is associated to it, 0 is returned.
	 */
	int shortcut_low(ScanCode scan_code) const
	{
		auto it = _shortcut_low.find(scan_code);
		return it==_shortcut_low.end() ? 0 : it->second;
	}

	/**
	 * Return the index of the high-position shortcut associated to the key corresponding
	 * to the given scan-code. If the scan-code does not refer to a key, or if no
	 * shortcut is associated to it, 0 is returned.
	 */
	int shortcut_high(ScanCode scan_code) const
	{
		auto it = _shortcut_high.find(scan_code);
		return it==_shortcut_high.end() ? 0 : it->second;
	}

	/**
	 * Return the index of the (either low- or high-position depending on the flag `high_position`)
	 * shortcut associated to the key corresponding to the given scan-code.
	 */
	int shortcut(ScanCode scan_code, bool high_position) const
	{
		return high_position ? shortcut_high(scan_code) : shortcut_low(scan_code);
	}

	/**
	 * Reset the shortcut manager to its default state: no shortcut is associated to
	 * the keys, and an invalide scan-code is associated to the modifier keys.
	 */
	void reset();

	/**
	 * Configure the shortcut manager so that its state corresponds to the given
	 * modifier keys, keyboard map and shortcut map.
	 */
	void reset(ModifierKeys modifier_keys, const KeyboardMap &keyboard_map, const ShortcutMap &shortcut_map);

private:

	// Private functions
	bool update_modifier_key_scan_code(ScanCode scan_code, const std::string &id,
		const char *expected_id_for_left, const char *expected_id_for_right);

	// Private members
	Enum::array<Side, ScanCode> _modifier_key ;
	std::map<ScanCode, int>     _shortcut_low ;
	std::map<ScanCode, int>     _shortcut_high;
};

#endif /* SHORTCUTMANAGER_H_ */

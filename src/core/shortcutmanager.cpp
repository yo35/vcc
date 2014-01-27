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


#include "shortcutmanager.h"


// Constructor.
ShortcutManager::ShortcutManager() : _modifier_key({0, 0}) {}


// Return to the default state.
void ShortcutManager::reset()
{
	_modifier_key[Side::LEFT ] = 0;
	_modifier_key[Side::RIGHT] = 0;
	_shortcut_low .clear();
	_shortcut_high.clear();
}


// Configuration from "top-level" objects
void ShortcutManager::reset(ModifierKeys modifier_keys, const KeyboardMap &keyboard_map, const ShortcutMap &shortcut_map)
{
	reset();

	// For all registered key in the keyboard map...
	for(std::size_t k=0; k<keyboard_map.key_count(); ++k)
	{
		ScanCode scan_code = keyboard_map.key(k).scan_code();
		const std::string &id = keyboard_map.key(k).id();

		// Update the modifier key scan-codes.
		bool is_modifier_key = false;
		switch(modifier_keys) {
			case ModifierKeys::DOUBLE_CTRL : is_modifier_key=update_modifier_key_scan_code(scan_code, id, "ctrl-left" , "ctrl-right" ); break;
			case ModifierKeys::DOUBLE_SHIFT: is_modifier_key=update_modifier_key_scan_code(scan_code, id, "shift-left", "shift-right"); break;
			case ModifierKeys::DOUBLE_ALT  : is_modifier_key=update_modifier_key_scan_code(scan_code, id, "alt-left"  , "alt-right"  ); break;
			default: break;
		}

		// Retrieve the shortcuts associated to the key in the shortcut map. However, if the current key
		// is a modifier key, no shortcut is associated to it, whatever it is specified in the shortcup map.
		if(!is_modifier_key) {
			_shortcut_low [scan_code] = shortcut_map.shortcut_low (id);
			_shortcut_high[scan_code] = shortcut_map.shortcut_high(id);
		}
	}
}


// Auxiliary function: if `id` is equal to either `expected_id_for_left` or `expected_id_for_right`,
// the corresponding flag in `_modifier_key` is updated with the given scan-code.
bool ShortcutManager::update_modifier_key_scan_code(ScanCode scan_code, const std::string &id,
	const char *expected_id_for_left, const char *expected_id_for_right)
{
	if(id==expected_id_for_left) {
		_modifier_key[Side::LEFT] = scan_code;
		return true;
	}
	else if(id==expected_id_for_right) {
		_modifier_key[Side::RIGHT] = scan_code;
		return true;
	}
	else {
		return false;
	}
}

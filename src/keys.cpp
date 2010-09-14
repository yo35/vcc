/*
 *    This file is part of Virtual Chess Clock, a chess clock software
 *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keys.h"

std::string keyval_to_string(Keyval val) {
	return std::string(gdk_keyval_name(val));
}

KeyvalList keycode_to_keyvals(Keycode code) {
	KeyvalList retval;
	guint     *keyvals;
	gint       n_entries;
	if(gdk_keymap_get_entries_for_keycode(NULL, code, NULL, &keyvals, &n_entries)) {
		for(int k=0; k<n_entries; ++k) {
			retval.push_back(keyvals[k]);
		}
		g_free(keyvals);
	}
	return retval;
}

Keyval keycode_to_cannonical_keyval(Keycode code) {
	KeyvalList keyvals = keycode_to_keyvals(code);
	for(KeyvalList::const_iterator it=keyvals.begin(); it!=keyvals.end(); ++it) {
		if((*it>='0' && *it<='9') || (*it>='A' && *it<='Z'))
			return *it;
	}
	if(keyvals.empty())
		return 0;
	else
		return keyvals.front();
}

KeycodeList keyval_to_keycodes(Keyval val) {
	KeycodeList   retval;
	GdkKeymapKey *keys;
	gint          n_keys;
	if(gdk_keymap_get_entries_for_keyval(NULL, val, &keys, &n_keys)) {
		for(int k=0; k<n_keys; ++k) {
			retval.push_back(keys[k].keycode);
		}
		g_free(keys);
	}
	return retval;
}

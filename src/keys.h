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

#ifndef KEYS_H
#define KEYS_H

#include <gdk/gdk.h>
#include <string>
#include <list>


// Aliases
typedef guint              Keycode    ;
typedef guint              Keyval     ;
typedef std::list<Keycode> KeycodeList;
typedef std::list<Keyval > KeyvalList ;

// Nom associé à la keyval
std::string keyval_to_string(Keyval val);

// Conversions keycode vers keyval
KeyvalList keycode_to_keyvals(Keycode code);
Keyval     keycode_to_cannonical_keyval(Keycode code);

// Récupération du ou des keycodes à partir du keyval
KeycodeList keyval_to_keycodes(Keyval val);


#endif

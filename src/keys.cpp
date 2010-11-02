/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
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


#include "keys.h"
#include <gdk/gdkkeysyms.h>
#include <translation.h>

std::string keyval_to_string(Keyval val) {
	return std::string(gdk_keyval_name(val));
}

//KeyvalList keycode_to_keyvals(Keycode code) {
//	KeyvalList retval;
//	guint     *keyvals;
//	gint       n_entries;
//	if(gdk_keymap_get_entries_for_keycode(NULL, code, NULL, &keyvals, &n_entries)) {
//		for(int k=0; k<n_entries; ++k) {
//			retval.push_back(keyvals[k]);
//		}
//		g_free(keyvals);
//	}
//	return retval;
//}
//
//Keyval keycode_to_cannonical_keyval(Keycode code) {
//	KeyvalList keyvals = keycode_to_keyvals(code);
//	for(KeyvalList::const_iterator it=keyvals.begin(); it!=keyvals.end(); ++it) {
//		if((*it>='0' && *it<='9') || (*it>='A' && *it<='Z'))
//			return *it;
//	}
//	if(keyvals.empty())
//		return 0;
//	else
//		return keyvals.front();
//}
//

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

Glib::ustring keyval_to_symbol(Keyval val) {

	// Caractères alpha-numériques classiques
	if((val>=GDK_a && val<=GDK_z) || (val>=GDK_A && val<=GDK_Z) || (val>=GDK_0 && val<=GDK_9))
	{
		return keyval_to_string(val);
	}

	// Touches F1, F2, etc...
	if(val>=GDK_F1 && val<=GDK_F35) {
		return keyval_to_string(val);
	}

	// Autres
	switch(val) {

		// Symboles ASCII imprimables
		case GDK_exclam      : return "!" ;
		case GDK_quotedbl    : return "\"";
		case GDK_numbersign  : return "#" ;
		case GDK_dollar      : return "$" ;
		case GDK_percent     : return "%" ;
		case GDK_ampersand   : return "&" ;
		case GDK_apostrophe  : return "'" ;
		case GDK_parenleft   : return "(" ;
		case GDK_parenright  : return ")" ;
		case GDK_asterisk    : return "*" ;
		case GDK_plus        : return "+" ;
		case GDK_comma       : return "," ;
		case GDK_minus       : return "-" ;
		case GDK_period      : return "." ;
		case GDK_slash       : return "/" ;
		case GDK_colon       : return ":" ;
		case GDK_semicolon   : return ";" ;
		case GDK_less        : return "<" ;
		case GDK_equal       : return "=" ;
		case GDK_greater     : return ">" ;
		case GDK_question    : return "?" ;
		case GDK_at          : return "@" ;
		case GDK_bracketleft : return "[" ;
		case GDK_backslash   : return "\\";
		case GDK_bracketright: return "]" ;
		case GDK_asciicircum : return "^" ;
		case GDK_underscore  : return "_" ;
		case GDK_grave       : return "`" ;
		case GDK_braceleft   : return "{" ;
		case GDK_bar         : return "|" ;
		case GDK_braceright  : return "}" ;
		case GDK_asciitilde  : return "~" ;

		// Touches de contrôle non-symétriques sur le clavier principal
		case GDK_space     : return _("Space"     );
		case GDK_Caps_Lock : return _("Caps lock" );
		case GDK_Shift_Lock: return _("Shift lock");
		case GDK_Escape    : return _("Esc"       );
		case GDK_Return    : return _("Return"    );
		case GDK_BackSpace : return _("Backspace" );
		case GDK_Tab       : return _("Tabulation");


		//case GDK_Delete    : return _("Del"       );


		// Touches de contrôle symétriques
		#define  LEFT_KEY_NAME(name) GDK_ ## name ## _L
		#define RIGHT_KEY_NAME(name) GDK_ ## name ## _R
		case LEFT_KEY_NAME(Shift  ): case RIGHT_KEY_NAME(Shift  ): return _("Shift"  );
		case LEFT_KEY_NAME(Control): case RIGHT_KEY_NAME(Control): return _("Control");
		case LEFT_KEY_NAME(Alt    ): case RIGHT_KEY_NAME(Alt    ): return _("Alt"    );
		case LEFT_KEY_NAME(Meta   ): case RIGHT_KEY_NAME(Meta   ): return _("Meta"   );
		case LEFT_KEY_NAME(Super  ): case RIGHT_KEY_NAME(Super  ): return _("Super"  );
		case LEFT_KEY_NAME(Hyper  ): case RIGHT_KEY_NAME(Hyper  ): return _("Hyper"  );

		// Solution par défaut
		default:
			return keyval_to_string(val);
	}
}

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

// Constructeur
PhysicalKey::PhysicalKey() {
	m_bottom_line = 0;
}

// Accesseurs
int      PhysicalKey::bottom_line  ()        const { return m_bottom_line     ; }
int      PhysicalKey::nb_lines     ()        const { return m_geometry.size (); }
int      PhysicalKey::nb_keyvals   ()        const { return m_keyval_ex.size(); }
int      PhysicalKey::pos_on_line  (int idx) const { return m_geometry [idx].pos   ; }
int      PhysicalKey::width_on_line(int idx) const { return m_geometry [idx].width ; }
Keyval   PhysicalKey::keyval       (int idx) const { return m_keyval_ex[idx].keyval; }
KeyGroup PhysicalKey::group        (int idx) const { return m_keyval_ex[idx].group ; }
KeyLevel PhysicalKey::level        (int idx) const { return m_keyval_ex[idx].level ; }

// Modifieurs
void PhysicalKey::set_bottom_line(int src) { m_bottom_line = src; }
void PhysicalKey::set_nb_lines   (int src) { m_geometry .resize(src); }
void PhysicalKey::set_nb_keyvals (int src) { m_keyval_ex.resize(src); }
void PhysicalKey::set_geometry(int idx, int pos, int width) {
	m_geometry[idx].pos   = pos  ;
	m_geometry[idx].width = width;
}
void PhysicalKey::set_keyval(int idx, Keyval keyval, KeyGroup group, KeyLevel level) {
	m_keyval_ex[idx].keyval = keyval;
	m_keyval_ex[idx].group  = group ;
	m_keyval_ex[idx].level  = level ;
}

// Keycodes associés à 1 keyval
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

// Keyvals assocés à 1 keycode
PhysicalKey keycode_to_keyvals(Keycode code) {
	PhysicalKey   retval;
	GdkKeymapKey *grouplevels;
	Keyval       *keyvals;
	gint          n_entries;
	if(gdk_keymap_get_entries_for_keycode(NULL, code, &grouplevels, &keyvals, &n_entries)) {
		retval.set_nb_keyvals(n_entries);
		for(int k=0; k<n_entries; ++k) {
			retval.set_keyval(k, keyvals[k], grouplevels[k].group, grouplevels[k].level);
		}
		g_free(grouplevels);
		g_free(keyvals    );
	}
	return retval;
}

// Nom standard d'un keyval
std::string keyval_to_string(Keyval val) {
	return std::string(gdk_keyval_name(val));
}

// Nom "affichable" d'un keyval
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

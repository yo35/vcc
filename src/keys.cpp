/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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

// Keyval étendu
KeyvalEx::KeyvalEx()                                    { keyval=0 ; group=0 ; level=0 ; }
KeyvalEx::KeyvalEx(Keyval kv, KeyGroup kg, KeyLevel kl) { keyval=kv; group=kg; level=kl; }

// Constructeur
PhysicalKey::PhysicalKey() {
	m_bottom_line = 0;
}

// Accesseurs
int      PhysicalKey::bottom_line  ()        const { return m_bottom_line     ; }
bool     PhysicalKey::in_kp        ()        const { return m_in_kp           ; }
int      PhysicalKey::nb_lines     ()        const { return m_geometry.size (); }
int      PhysicalKey::nb_keyvals   ()        const { return m_keyval_ex.size(); }
int      PhysicalKey::pos_on_line  (int idx) const { return m_geometry [idx].pos   ; }
int      PhysicalKey::width_on_line(int idx) const { return m_geometry [idx].width ; }
Keyval   PhysicalKey::keyval       (int idx) const { return m_keyval_ex[idx].keyval; }
KeyGroup PhysicalKey::group        (int idx) const { return m_keyval_ex[idx].group ; }
KeyLevel PhysicalKey::level        (int idx) const { return m_keyval_ex[idx].level ; }
bool     PhysicalKey::print        (int idx) const { return m_keyval_ex[idx].print ; }

// Modifieurs
void PhysicalKey::set_bottom_line(int src ) { m_bottom_line = src; }
void PhysicalKey::set_in_kp      (bool src) { m_in_kp       = src; }
void PhysicalKey::set_nb_lines   (int src ) { m_geometry .resize(src); }
void PhysicalKey::set_nb_keyvals (int src ) { m_keyval_ex.resize(src); }
void PhysicalKey::set_geometry(int idx, int pos, int width) {
	m_geometry[idx].pos   = pos  ;
	m_geometry[idx].width = width;
}
void PhysicalKey::set_keyval(int idx, Keyval keyval, KeyGroup group,
	KeyLevel level, bool print)
{
	m_keyval_ex[idx].keyval = keyval;
	m_keyval_ex[idx].group  = group ;
	m_keyval_ex[idx].level  = level ;
	m_keyval_ex[idx].print  = print ;
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
KeyvalExList keycode_to_keyvals(Keycode code) {
	KeyvalExList  retval;
	GdkKeymapKey *grouplevels;
	Keyval       *keyvals;
	gint          n_entries;
	if(gdk_keymap_get_entries_for_keycode(NULL, code, &grouplevels, &keyvals, &n_entries)) {
		for(int k=0; k<n_entries; ++k) {
			retval.push_back(KeyvalEx(keyvals[k], grouplevels[k].group, grouplevels[k].level));
		}
		g_free(grouplevels);
		g_free(keyvals    );
	}
	return retval;
}

// Nom standard d'un keyval
std::string keyval_to_string(Keyval val) {
	if(val==0)
		return "Null character";
	else
		return std::string(gdk_keyval_name(val));
}

// Nom "affichable" d'un keyval
Glib::ustring keyval_to_symbol(Keyval val) {

	// Caractères aplha-numérique usuels
	if((val>=GDK_a && val<=GDK_z) || (val>=GDK_A && val<=GDK_Z) || (val>=GDK_0 && val<=GDK_9)) {
		return keyval_to_string(val);
	}

	// Touches F1, F2, etc...
	if(val>=GDK_F1 && val<=GDK_F35) {
		return keyval_to_string(val);
	}

	// Autres
	switch(val) {

		// Caractère nul
		case 0: return _("Null\ncharacter");

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
		case GDK_space           : return _("Space"      );
		case GDK_Caps_Lock       : return _("Caps\nlock" );
		case GDK_Shift_Lock      : return _("Shift\nlock");
		case GDK_Escape          : return _("Esc"        );
		case GDK_Return          : return _("Return"     );
		case GDK_BackSpace       : return _("Backspace"  );
		case GDK_Tab             : return _("Tabs"       );
		case GDK_ISO_Level3_Shift: return _("Alt Gr"     );

		// Touches de navigation
		case GDK_Delete   : case GDK_KP_Delete   : return _("Del"       );
		case GDK_Insert   : case GDK_KP_Insert   : return _("Ins"       );
		case GDK_Page_Up  : case GDK_KP_Page_Up  : return _("Page\nup"  );
		case GDK_Page_Down: case GDK_KP_Page_Down: return _("Page\ndown");
		case GDK_Home     : case GDK_KP_Home     : return _("Home"      );
		case GDK_End      : case GDK_KP_End      : return _("End"       );
		case GDK_Left     : case GDK_KP_Left     : return "←";
		case GDK_Right    : case GDK_KP_Right    : return "→";
		case GDK_Up       : case GDK_KP_Up       : return "↑";
		case GDK_Down     : case GDK_KP_Down     : return "↓";

		// Touche de contrôle spéciales
		case GDK_3270_PrintScreen: return _("Print\nscreen");
		case GDK_Scroll_Lock     : return _("Scroll\nlock" );
		case GDK_Pause           : return _("Pause"        );

		// Touches de contrôle symétriques
		#define  LEFT_KEY_NAME(name) GDK_ ## name ## _L
		#define RIGHT_KEY_NAME(name) GDK_ ## name ## _R
		case LEFT_KEY_NAME(Shift  ): case RIGHT_KEY_NAME(Shift  ): return _("Shift");
		case LEFT_KEY_NAME(Control): case RIGHT_KEY_NAME(Control): return _("Ctrl" );
		case LEFT_KEY_NAME(Alt    ): case RIGHT_KEY_NAME(Alt    ): return _("Alt"  );
		case LEFT_KEY_NAME(Meta   ): case RIGHT_KEY_NAME(Meta   ): return _("Meta" );
		case LEFT_KEY_NAME(Super  ): case RIGHT_KEY_NAME(Super  ): return _("Super");
		case LEFT_KEY_NAME(Hyper  ): case RIGHT_KEY_NAME(Hyper  ): return _("Hyper");

		// Pavé numérique
		case GDK_KP_0       : return "0";
		case GDK_KP_1       : return "1";
		case GDK_KP_2       : return "2";
		case GDK_KP_3       : return "3";
		case GDK_KP_4       : return "4";
		case GDK_KP_5       : return "5";
		case GDK_KP_6       : return "6";
		case GDK_KP_7       : return "7";
		case GDK_KP_8       : return "8";
		case GDK_KP_9       : return "9";
		case GDK_KP_Add     : return "+";
		case GDK_KP_Subtract: return "-";
		case GDK_KP_Multiply: return "*";
		case GDK_KP_Divide  : return "/";
		case GDK_KP_Begin   : return "" ;
		case GDK_KP_Enter   : return _("Return"       );
		case GDK_Num_Lock   : return _("Num\nlock"    );
		case GDK_KP_Decimal : return _("Decimal\nmark");

		// Caractères ISO 8859 (de 0xA0 à 0xFF)
		case GDK_nobreakspace  : return _("No break\nspace");
		case GDK_exclamdown    : return "¡";
		case GDK_cent          : return "¢";
		case GDK_sterling      : return "£";
		case GDK_currency      : return "¤";
		case GDK_yen           : return "¥";
		case GDK_brokenbar     : return "¦";
		case GDK_section       : return "§";
		case GDK_diaeresis     : return "¨";
		case GDK_copyright     : return "©";
		case GDK_ordfeminine   : return "ª";
		case GDK_guillemotleft : return "«";
		case GDK_notsign       : return "¬";
		case GDK_hyphen        : return _("Hyphen");
		case GDK_registered    : return "®";
		case GDK_macron        : return "¯";
		case GDK_degree        : return "°";
		case GDK_plusminus     : return "±";
		case GDK_twosuperior   : return "²";
		case GDK_threesuperior : return "³";
		case GDK_acute         : return "´";
		case GDK_mu            : return "µ";
		case GDK_paragraph     : return "¶";
		case GDK_periodcentered: return "⋅";
		case GDK_cedilla       : return "¸";
		case GDK_onesuperior   : return "¹";
		case GDK_masculine     : return "º";
		case GDK_guillemotright: return "»";
		case GDK_onequarter    : return "¼";
		case GDK_onehalf       : return "½";
		case GDK_threequarters : return "¾";
		case GDK_questiondown  : return "¿";
		case GDK_Agrave        : return "À";
		case GDK_Aacute        : return "Á";
		case GDK_Acircumflex   : return "Â";
		case GDK_Atilde        : return "Ã";
		case GDK_Adiaeresis    : return "Ä";
		case GDK_Aring         : return "Å";
		case GDK_AE            : return "Æ";
		case GDK_Ccedilla      : return "Ç";
		case GDK_Egrave        : return "È";
		case GDK_Eacute        : return "É";
		case GDK_Ecircumflex   : return "Ê";
		case GDK_Ediaeresis    : return "Ë";
		case GDK_Igrave        : return "Ì";
		case GDK_Iacute        : return "Í";
		case GDK_Icircumflex   : return "Î";
		case GDK_Idiaeresis    : return "Ï";
		case GDK_Eth           : return "Ð";
		case GDK_Ntilde        : return "Ñ";
		case GDK_Ograve        : return "Ò";
		case GDK_Oacute        : return "Ó";
		case GDK_Ocircumflex   : return "Ô";
		case GDK_Otilde        : return "Õ";
		case GDK_Odiaeresis    : return "Ö";
		case GDK_multiply      : return "×";
		case GDK_Ooblique      : return "Ø";
		case GDK_Ugrave        : return "Ù";
		case GDK_Uacute        : return "Ú";
		case GDK_Ucircumflex   : return "Û";
		case GDK_Udiaeresis    : return "Ü";
		case GDK_Yacute        : return "Ý";
		case GDK_Thorn         : return "Þ";
		case GDK_ssharp        : return "ß";
		case GDK_agrave        : return "à";
		case GDK_aacute        : return "á";
		case GDK_acircumflex   : return "â";
		case GDK_atilde        : return "ã";
		case GDK_adiaeresis    : return "ä";
		case GDK_aring         : return "å";
		case GDK_ae            : return "æ";
		case GDK_ccedilla      : return "ç";
		case GDK_egrave        : return "è";
		case GDK_eacute        : return "é";
		case GDK_ecircumflex   : return "ê";
		case GDK_ediaeresis    : return "ë";
		case GDK_igrave        : return "ì";
		case GDK_iacute        : return "í";
		case GDK_icircumflex   : return "î";
		case GDK_idiaeresis    : return "ï";
		case GDK_eth           : return "ð";
		case GDK_ntilde        : return "ñ";
		case GDK_ograve        : return "ò";
		case GDK_oacute        : return "ó";
		case GDK_ocircumflex   : return "ô";
		case GDK_otilde        : return "õ";
		case GDK_odiaeresis    : return "ö";
		case GDK_division      : return "÷";
		case GDK_ooblique      : return "ø";
		case GDK_ugrave        : return "ù";
		case GDK_uacute        : return "ú";
		case GDK_ucircumflex   : return "û";
		case GDK_udiaeresis    : return "ü";
		case GDK_yacute        : return "ý";
		case GDK_thorn         : return "þ";
		case GDK_ydiaeresis    : return "ÿ";

		// Touches mortes
		case GDK_dead_grave      : return "`";
		case GDK_dead_acute      : return "´";
		case GDK_dead_circumflex : return "^";
		case GDK_dead_tilde      : return "~";
		case GDK_dead_macron     : return "¯";
		case GDK_dead_diaeresis  : return "¨";
		case GDK_dead_caron      : return "ˇ";
		case GDK_dead_cedilla    : return "¸";
		case GDK_dead_doubleacute: return "˝";
		case GDK_dead_abovering  : return "˚";
		case GDK_dead_abovedot   : return "·";
		case GDK_dead_breve      : return "˘";
		case GDK_dead_ogonek     : return "˛";

		// Caractères latins diacritiques minuscule (hors-latin1)
		case GDK_scedilla : return "ş​";
		case GDK_gbreve   : return "ğ";
		case GDK_dstroke  : return "đ";
		case GDK_lstroke  : return "ł";
		case GDK_uring    : return "ů";
		case GDK_ccaron   : return "č";
		case GDK_dcaron   : return "ď​";
		case GDK_ecaron   : return "ě";
		case GDK_lcaron   : return "ľ";
		case GDK_ncaron   : return "ň";
		case GDK_rcaron   : return "ř";
		case GDK_scaron   : return "š";
		case GDK_tcaron   : return "ť";
		case GDK_zcaron   : return "ž";
		case GDK_aogonek  : return "ą";
		case GDK_eogonek  : return "ę";
		case GDK_iogonek  : return "į";
		case GDK_uogonek  : return "ų";
		case GDK_umacron  : return "ū";
		case GDK_eabovedot: return "ė";
		case GDK_racute   : return "ŕ";

		// Caractères latins diacritiques majuscule (hors-latin1)
		case GDK_Scedilla : return "Ş";
		case GDK_Gbreve   : return "Ğ";
		case GDK_Dstroke  : return "Đ";
		case GDK_Lstroke  : return "Ł";
		case GDK_Uring    : return "Ů";
		case GDK_Ccaron   : return "Č";
		case GDK_Dcaron   : return "Ď";
		case GDK_Ecaron   : return "Ě";
		case GDK_Lcaron   : return "Ľ";
		case GDK_Ncaron   : return "Ň";
		case GDK_Rcaron   : return "Ř";
		case GDK_Scaron   : return "Š";
		case GDK_Tcaron   : return "Ť";
		case GDK_Zcaron   : return "Ž";
		case GDK_Aogonek  : return "Ą";
		case GDK_Eogonek  : return "Ę";
		case GDK_Iogonek  : return "Į";
		case GDK_Uogonek  : return "Ų";
		case GDK_Umacron  : return "Ū";
		case GDK_Eabovedot: return "Ė";
		case GDK_Racute   : return "Ŕ";

		// Divers
		case GDK_EuroSign : return "€";
		case GDK_Iabovedot: return "İ";
		case GDK_idotless : return "ı";

		// Solution par défaut
		default:
			return keyval_to_string(val);
	}
}

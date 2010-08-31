
#ifndef KEYMAP_H
#define KEYMAP_H

#include <gdk/gdk.h>
#include <string>
#include <list>


namespace Keymap {

	// Alias
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
}

#endif

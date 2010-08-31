
#ifndef KEYMAP_H
#define KEYMAP_H

#include <gdk/gdk.h>
#include <string>
#include <list>


namespace Keymap {

	// Alias
	typedef guint Keycode;
	typedef guint Keyval;

	// Nom associé à la keyval
	std::string keyval_to_string(Keyval val);

	// Conversions keycode vers keyval
	std::list<Keyval> keycode_to_keyvals(Keycode code);
	Keyval            keycode_to_cannonical_keyval(Keycode code);

	// Récupération du ou des keycodes à partir du keyval
	std::list<Keycode> keyval_to_keycodes(Keyval val);
}

#endif

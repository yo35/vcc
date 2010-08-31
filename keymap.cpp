
#include "keymap.h"

std::string Keymap::keyval_to_string(Keyval val) {
	return std::string(gdk_keyval_name(val));
}

std::list<Keymap::Keyval> Keymap::keycode_to_keyvals(Keycode code) {
	std::list<Keymap::Keyval> retval;
	guint *keyvals;
	gint   n_entries;
	if(gdk_keymap_get_entries_for_keycode(NULL, code, NULL, &keyvals, &n_entries)) {
		for(int k=0; k<n_entries; ++k) {
			retval.push_back(keyvals[k]);
		}
		g_free(keyvals);
	}
	return retval;
}

Keymap::Keyval Keymap::keycode_to_cannonical_keyval(Keycode code) {
	std::list<Keyval> keyvals = keycode_to_keyvals(code);
	for(std::list<Keyval>::const_iterator it=keyvals.begin(); it!=keyvals.end(); ++it) {
		if((*it>='0' && *it<='9') || (*it>='A' && *it<='Z'))
			return *it;
	}
	if(keyvals.empty())
		return 0;
	else
		return keyvals.front();
}

std::list<Keymap::Keycode> Keymap::keyval_to_keycodes(Keyval val) {
	std::list<Keycode> retval;
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

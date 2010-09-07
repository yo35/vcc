
#include "icon.h"

#define PATH_TO_ICON "data/"

Icon::Icon(const std::string &filename) {
	path = PATH_TO_ICON + filename;
}

Glib::RefPtr<Gdk::Pixbuf> Icon::get(int sz) {
	if(!data[sz])
		data[sz] = Gdk::Pixbuf::create_from_file(path, sz, sz);
	return data[sz];
}

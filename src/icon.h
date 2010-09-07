
#ifndef ICON_H
#define ICON_H

#include <string>
#include <map>
#include <gdkmm/pixbuf.h>


class Icon {

public:

	// Constructeur
	explicit Icon(const std::string &filename);

	// Accès à la ressource
	Glib::RefPtr<Gdk::Pixbuf> get(int sz);

private:

	// Alias
	typedef std::map<int, Glib::RefPtr<Gdk::Pixbuf> > PixbufMap;

	// Données membres
	std::string path;
	PixbufMap   data;
};

#endif

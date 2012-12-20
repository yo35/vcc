/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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

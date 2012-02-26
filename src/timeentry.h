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


#ifndef TIMEENTRY_H
#define TIMEENTRY_H

#include <gtkmm/spinbutton.h>
#include <gtkmm/box.h>

class TimeEntry : public Gtk::HBox {

public:

	// Construction
	TimeEntry();
	sigc::signal<void> signal_changed() const;

	// Modification
	void set_sensitive(bool sensitive);
	void set_time(int new_time);
	int  get_time() const;

private:

	// Signaux
	sigc::signal<void> m_signal_changed;

	// Widgets
	Gtk::SpinButton hour;
	Gtk::SpinButton min ;
	Gtk::SpinButton sec ;
	Gtk::Label      lbl_hour;
	Gtk::Label      lbl_min ;
	Gtk::Label      lbl_sec ;
};

#endif

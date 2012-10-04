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


#ifndef NAMEDIALOG_H_
#define NAMEDIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include "side.h"

/**
 * Dialog to define the name of the players and to select whether they should
 * be visible or not
 */
class NameDialog : public Gtk::Dialog
{
public:

	/**
	 * Constructor
	 */
	NameDialog(Gtk::Window &parent);

	/**
	 * Retrieve the selected name of a player
	 */
	std::string name(Side side) const;

	/**
	 * Set the default name of a player
	 */
	void set_name(Side side, const std::string &value);

	/**
	 * Whether the names should be visible or not
	 */
	bool visible_names() const;

	/**
	 * Set the default choice for name visibility
	 */
	void set_visible_names(bool value);

private:

	// Private functions
	void on_switch_clicked();
	void refresh_sensitivity();

	// Member objects
	EnumArray<Side, Gtk::Label> m_label  ;
	EnumArray<Side, Gtk::Entry> m_name   ;
	Gtk::Button                 m_switch ;
	Gtk::CheckButton            m_visible;
	Gtk::Table                  m_main_layout;
	Gtk::HBox                   m_sub_layout ;
};

#endif /* NAMEDIALOG_H_ */

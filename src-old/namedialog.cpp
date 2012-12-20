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


#include "namedialog.h"
#include <translation.h>
#include <gtkmm/stock.h>

// Constructor
NameDialog::NameDialog(Gtk::Window &parent) : Gtk::Dialog(_("Players' names"), parent, true, true),
	m_switch (_("Switch players")),
	m_visible(_("Display players' names")),
	m_main_layout(2, 2)
{
	// Response buttons
	add_button(Gtk::Stock::OK    , Gtk::RESPONSE_OK    );
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	set_default_response(Gtk::RESPONSE_OK);

	// Widgets
	m_label[LEFT ].set_text(_("Left player" ));
	m_label[RIGHT].set_text(_("Right player"));
	m_switch .signal_clicked().connect(sigc::mem_fun(*this, &NameDialog::on_switch_clicked  ));
	m_visible.signal_clicked().connect(sigc::mem_fun(*this, &NameDialog::refresh_sensitivity));
	refresh_sensitivity();

	// Geometry
	m_main_layout.set_spacings(5);
	m_sub_layout.set_spacing(5);
	get_vbox()->set_spacing(5);
	m_main_layout.attach(m_label[LEFT ], 0, 1, 0, 1);
	m_main_layout.attach(m_label[RIGHT], 1, 2, 0, 1);
	m_main_layout.attach(m_name [LEFT ], 0, 1, 1, 2);
	m_main_layout.attach(m_name [RIGHT], 1, 2, 1, 2);
	m_sub_layout.pack_start(m_switch, Gtk::PACK_SHRINK);
	m_sub_layout.pack_start(m_visible);
	get_vbox()->pack_start(m_main_layout, Gtk::PACK_SHRINK);
	get_vbox()->pack_start(m_sub_layout , Gtk::PACK_SHRINK);
	show_all_children();
}

/**
 * Retrieve the selected name of a player
 */
std::string NameDialog::name(Side side) const
{
	return m_name[side].get_text();
}

/**
 * Set the default name of a player
 */
void NameDialog::set_name(Side side, const std::string &value)
{
	m_name[side].set_text(value);
}

/**
 * Whether the names should be visible or not
 */
bool NameDialog::visible_names() const
{
	return m_visible.get_active();
}

/**
 * Set the default choice for name visibility
 */
void NameDialog::set_visible_names(bool value)
{
	m_visible.set_active(value);
}

/**
 * Switch the names
 */
void NameDialog::on_switch_clicked()
{
	std::string buffer = m_name[LEFT].get_text();
	m_name[LEFT ].set_text(m_name[RIGHT].get_text());
	m_name[RIGHT].set_text(buffer);
}

/**
 * Refresh the text fields sensitivity according to the check button 'visible' state
 */
void NameDialog::refresh_sensitivity()
{
	bool visible = m_visible.get_active();
	for(Side::iterator s=Side::first(); s.valid(); ++s) {
		m_name[*s].set_sensitive(visible);
	}
	m_switch.set_sensitive(visible);
}

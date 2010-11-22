/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
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


#include "debugdialog.h"
#include "params.h"
#include <translation.h>
#include <gtkmm/stock.h>

// Constructeur
DebugDialog::DebugDialog(Gtk::Window &parent) :
	Gtk::Dialog(_("Debug information"), parent, true, true)
{
	// Boutons de réponse
	add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);
	set_default_response(Gtk::RESPONSE_CLOSE);

	// Widgets d'info
	infos_label[0].set_label(_("Current directory" ));
	infos_label[1].set_label(_("Top directory"     ));
	infos_label[2].set_label(_("User directory"    ));
	infos_label[3].set_label(_("Locale"            ));
	infos[0].set_text(Glib::get_current_dir());
	infos[1].set_text(gp->prefix_path      ());
	infos[2].set_text(gp->config_path      ());
	infos[3].set_text(gp->locale           ());

	// Configuration générale des widget d'info
	for(unsigned int k=0; k<4; ++k) {
		infos       [k].set_editable(false);
		infos_layout[k].set_spacing(5);
		infos_layout[k].pack_start(infos_label[k], Gtk::PACK_SHRINK);
		infos_layout[k].pack_start(infos      [k]);
		get_vbox()->pack_start(infos_layout[k], Gtk::PACK_SHRINK);
	}

	// Géométrie générale
	get_vbox()->set_spacing(5);
	show_all_children();
}

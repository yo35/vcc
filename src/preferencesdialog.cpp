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


#include "preferencesdialog.h"
#include "params.h"
#include <gtkmm/stock.h>
#include <translation.h>

// Constructeur
PreferencesDialog::PreferencesDialog(Gtk::Window &parent) :
	Gtk::Dialog(_("Preferences"), parent, true, true)
{
	// Boutons de réponse
	add_button(Gtk::Stock::OK    , Gtk::RESPONSE_OK    );
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	set_default_response(Gtk::RESPONSE_OK);

	// Frame "RAZ par toolbar"
	raz_by_toolbar.set_label(_("Reset through the toolbar"));
	raz_by_toolbar_layout.set_border_width(5);
	raz_by_toolbar_layout.set_spacing(5);
	ask_before_raz_label.set_label(_("Ask for confirmation before resetting"));
	//ask_before_raz_label.set_justify(Gtk::JUSTIFY_LEFT);
	raz_by_toolbar_layout.pack_start(ask_before_raz_label);
	ask_before_raz[RC_ALWAYS       ].set_label(_("Always"                            ));
	ask_before_raz[RC_IF_NOT_PAUSED].set_label(_("Only if the clock is still running"));
	ask_before_raz[RC_NEVER        ].set_label(_("Never"                             ));
	for(ReinitConfirm::iterator k=ReinitConfirm::first(); k.valid(); ++k) {
		ask_before_raz[*k].set_group(ask_before_raz_group);
		raz_by_toolbar_layout.pack_start(ask_before_raz[*k]);
	}
	raz_by_toolbar.add(raz_by_toolbar_layout);

	// Frame "RAZ par clavier"
	raz_by_keyboard.set_label(_("Reset through the keyboard"));
	raz_by_keyboard_layout.set_border_width(5);
  raz_by_keyboard_layout.set_spacing(5);
	key_combination_label.set_label(_("Keys to press to reset"));
	//key_combination_label.set_justify(Gtk::JUSTIFY_LEFT);
	raz_by_keyboard_layout.pack_start(key_combination_label);
	key_combination[DOUBLE_CTRL].set_label(_("CTRL keys (left and right)"));
	key_combination[DOUBLE_MAJ ].set_label(_("MAJ keys (left and right)" ));
	for(KeyCombination::iterator k=KeyCombination::first(); k.valid(); ++k) {
		key_combination[*k].set_group(key_combination_group);
		raz_by_keyboard_layout.pack_start(key_combination[*k]);
	}
	raz_delay_label.set_label(_("Delay"));
	raz_delay_layout.set_spacing(5);
	raz_delay.set_digits(0);
	raz_delay.set_range(0, 2000);
	raz_delay.set_increments(100, 500);
	raz_delay_layout.pack_start(raz_delay_label, Gtk::PACK_SHRINK);
	raz_delay_layout.pack_start(raz_delay);
	raz_by_keyboard_layout.pack_start(raz_delay_layout);
	raz_by_keyboard.add(raz_by_keyboard_layout);

	// Onglet RAZ
	raz_page.pack_start(raz_by_toolbar );
	raz_page.pack_start(raz_by_keyboard);
	pages.append_page(raz_page, _("Reset options"));

	// Géométrie générale
	get_vbox()->set_spacing(5);
	get_vbox()->pack_start(pages);
	show_all_children();
}

// Chargement des paramètres
void PreferencesDialog::load_params() {
	ask_before_raz [gp->reinit_confirm()].set_active(true);
	key_combination[gp->reinit_keys   ()].set_active(true);
	raz_delay.set_value(gp->reinit_delay());
}

// Enregistrement des paramètres
void PreferencesDialog::save_params() {
	for(ReinitConfirm::iterator k=ReinitConfirm::first(); k.valid(); ++k) {
		if(ask_before_raz[*k].get_active())
			gp->set_reinit_confirm(*k);
	}
	for(KeyCombination::iterator k=KeyCombination::first(); k.valid(); ++k) {
		if(key_combination[*k].get_active())
			gp->set_reinit_key(*k);
	}
	gp->set_reinit_delay(raz_delay.get_value());
}

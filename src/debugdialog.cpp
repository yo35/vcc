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
#include "strings.h"
#include <translation.h>
#include <gtkmm/stock.h>

// Modèle pour le widget d'affichage des infos clavier
Gtk::TreeModelColumn<Keyval       > DebugDialog::KeyInfoModel::keyval    () const { return m_keyval    ; }
Gtk::TreeModelColumn<std::string  > DebugDialog::KeyInfoModel::keyval_hex() const { return m_keyval_hex; }
Gtk::TreeModelColumn<std::string  > DebugDialog::KeyInfoModel::name      () const { return m_name      ; }
Gtk::TreeModelColumn<Glib::ustring> DebugDialog::KeyInfoModel::symbol    () const { return m_symbol    ; }
Gtk::TreeModelColumn<KeyGroup     > DebugDialog::KeyInfoModel::group     () const { return m_group     ; }
Gtk::TreeModelColumn<KeyLevel     > DebugDialog::KeyInfoModel::level     () const { return m_level     ; }
DebugDialog::KeyInfoModel::KeyInfoModel() : Gtk::TreeModelColumnRecord() {
	add(m_keyval    );
	add(m_keyval_hex);
	add(m_name      );
	add(m_symbol    );
	add(m_group     );
	add(m_level     );
}

// Constructeur
DebugDialog::DebugDialog(Gtk::Window &parent) :
	Gtk::Dialog(_("Debug information"), parent, true, true)
{
	// Divers
	set_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
	add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);

	// Widgets d'info
	m_geninfos_label[0].set_label(_("Current directory" ));
	m_geninfos_label[1].set_label(_("Top directory"     ));
	m_geninfos_label[2].set_label(_("User directory"    ));
	m_geninfos_label[3].set_label(_("Locale"            ));
	m_geninfos[0].set_text(Glib::get_current_dir());
	m_geninfos[1].set_text(gp->prefix_path      ());
	m_geninfos[2].set_text(gp->config_path      ());
	m_geninfos[3].set_text(gp->locale           ());
	m_geninfos_layout.resize(4, 2);
	m_geninfos_layout.set_spacings(5);
	for(unsigned int k=0; k<4; ++k) {
		m_geninfos[k].set_editable(false);
		m_geninfos_layout.attach(m_geninfos_label[k], 0, 1, k, k+1, Gtk::SHRINK|Gtk::FILL);
		m_geninfos_layout.attach(m_geninfos      [k], 1, 2, k, k+1);
	}

	// Affichage du keycode
	m_keycode_label .set_label(_("Keycode"));
	m_keycode       .set_editable(false);
	m_keycode_layout.set_spacing(5);
	m_keycode_layout.pack_start(m_keycode_label, Gtk::PACK_SHRINK);
	m_keycode_layout.pack_start(m_keycode);

	// Zone d'affichage des infos keyvals
	m_keyvals_data = Gtk::ListStore::create(m_keyvals_model);
	m_keyvals.set_model(m_keyvals_data);
	m_keyvals.append_column(_("Keyval"      ), m_keyvals_model.keyval    ());
	m_keyvals.append_column(_("Keyval (hex)"), m_keyvals_model.keyval_hex());
	m_keyvals.append_column(_("Name"        ), m_keyvals_model.name      ());
	m_keyvals.append_column(_("Symbol"      ), m_keyvals_model.symbol    ());
	m_keyvals.append_column(_("Group"       ), m_keyvals_model.group     ());
	m_keyvals.append_column(_("Level"       ), m_keyvals_model.level     ());
	m_keyvals.set_can_focus(false);
	for(unsigned int k=0; k<6; ++k) {
		m_keyvals.get_column(k)->set_resizable(true);
		m_keyvals.get_column(k)->set_min_width(60);
	}
	m_keyvals_scroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_keyvals_scroll.add(m_keyvals);

	// Frame keyboard
	m_keyboard_frame.set_label(_("Keyboard information"));
	m_keyboard_layout.set_border_width(5);
	m_keyboard_layout.set_spacing(5);
	m_keyboard_layout.pack_start(m_keycode_layout, Gtk::PACK_SHRINK);
	m_keyboard_layout.pack_start(m_keyvals_scroll);
	m_keyboard_frame.add(m_keyboard_layout);

	// Géométrie générale
	set_size_request(600, 500);
	get_vbox()->set_spacing(5);
	get_vbox()->pack_start(m_geninfos_layout, Gtk::PACK_SHRINK);
	get_vbox()->pack_start(m_keyboard_frame);
	show_all_children();
}

// Affichage des infos sur les touches pressées
bool DebugDialog::on_key_press_event(GdkEventKey* event) {

	// Récupération des keyvals associés à la touche courante
	KeyvalExList keyvals = keycode_to_keyvals(event->hardware_keycode);

	// Boucle d'affichage
	m_keyvals_data->clear();
	m_keycode.set_text(int_to_string(event->hardware_keycode));
	for(KeyvalExList::const_iterator it=keyvals.begin(); it!=keyvals.end(); ++it) {
		Gtk::TreeModel::iterator row = m_keyvals_data->append();
		(*row)[m_keyvals_model.keyval    ()] = it->keyval;
		(*row)[m_keyvals_model.keyval_hex()] = "0x" + int_to_string(it->keyval, true);
		(*row)[m_keyvals_model.name      ()] = keyval_to_string(it->keyval);
		(*row)[m_keyvals_model.symbol    ()] = keyval_to_symbol(it->keyval);
		(*row)[m_keyvals_model.group     ()] = it->group;
		(*row)[m_keyvals_model.level     ()] = it->level;
	}

	// Reprise du traitement normal
	return Gtk::Dialog::on_key_press_event(event);
}

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


#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include "keys.h"
#include <string>


class DebugDialog : public Gtk::Dialog {

public:

	// Constructeur
	DebugDialog(Gtk::Window &parent);

protected:

	// Callback pour les événements clavier
	virtual bool on_key_press_event(GdkEventKey* event);

private:

	// Modèle pour le widget d'affichage des infos clavier
	class KeyInfoModel : public Gtk::TreeModelColumnRecord {
		Gtk::TreeModelColumn<Keyval       > m_keyval    ;
		Gtk::TreeModelColumn<std::string  > m_keyval_hex;
		Gtk::TreeModelColumn<std::string  > m_name      ;
		Gtk::TreeModelColumn<Glib::ustring> m_symbol    ;
		Gtk::TreeModelColumn<KeyGroup     > m_group     ;
		Gtk::TreeModelColumn<KeyLevel     > m_level     ;
	public:
		Gtk::TreeModelColumn<Keyval       > keyval    () const;
    Gtk::TreeModelColumn<std::string  > keyval_hex() const;
    Gtk::TreeModelColumn<std::string  > name      () const;
    Gtk::TreeModelColumn<Glib::ustring> symbol    () const;
		Gtk::TreeModelColumn<KeyGroup     > group     () const;
		Gtk::TreeModelColumn<KeyLevel     > level     () const;
		KeyInfoModel();
	};

	// Données membres
	Gtk::Label                   m_geninfos_label [4];
	Gtk::Entry                   m_geninfos       [4];
	Gtk::Table                   m_geninfos_layout   ;
	Gtk::Frame                   m_keyboard_frame    ;
	Gtk::VBox                    m_keyboard_layout   ;
	Gtk::Label                   m_keycode_label     ;
	Gtk::Entry                   m_keycode           ;
	Gtk::HBox                    m_keycode_layout    ;
	KeyInfoModel                 m_keyvals_model     ;
	Glib::RefPtr<Gtk::ListStore> m_keyvals_data      ;
	Gtk::TreeView                m_keyvals           ;
	Gtk::ScrolledWindow          m_keyvals_scroll    ;
};

#endif

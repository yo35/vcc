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


#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <gtkmm/dialog.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/label.h>
#include <gtkmm/scale.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/liststore.h>
#include <gdkmm/pixbuf.h>
#include "optionenums.h"
#include "areamap.h"
#include "keyboardmapwidget.h"

class PreferencesDialog : public Gtk::Dialog {

public:

	// Constructeur
	PreferencesDialog(Gtk::Window &parent);

	// Chargement et write-back des options
	void load_params();
	void save_params();

protected:

	// Event handling
	virtual bool on_key_press_event  (GdkEventKey* event);
	virtual bool on_key_release_event(GdkEventKey* event);

private:

	// Callbacks
	void on_kb_changed();
	void on_display_kp_changed();
	void on_area_changed();
	void save_curr_area();
	void on_pause_enabled_clicked();

	// Modèle pour le combo box de sélection du clavier
	class KbSelectorModel : public Gtk::TreeModelColumnRecord {
		Gtk::TreeModelColumn<std::string               > m_code;
		Gtk::TreeModelColumn<Glib::ustring             > m_name;
		Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_icon;
	public:
		Gtk::TreeModelColumn<std::string               > code() const;
		Gtk::TreeModelColumn<Glib::ustring             > name() const;
		Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > icon() const;
		KbSelectorModel();
	};

	// Données membres
	Gtk::Notebook pages;
	std::string curr_kb_code;
	std::map<std::string, AreaMap> areas;

	// Onglet RAZ
	Gtk::VBox                                   raz_page;
	Gtk::Frame                                  raz_by_toolbar;
	Gtk::VBox                                   raz_by_toolbar_layout;
	Gtk::Label                                  ask_before_raz_label;
	Gtk::HBox                                   ask_before_raz_label_layout;
	Gtk::RadioButton::Group                     ask_before_raz_group;
	EnumArray<ReinitConfirm, Gtk::RadioButton>  ask_before_raz;
	Gtk::Frame                                  raz_by_keyboard;
	Gtk::VBox                                   raz_by_keyboard_layout;
	Gtk::Label                                  raz_combination_label;
	Gtk::HBox                                   raz_combination_label_layout;
	Gtk::RadioButton::Group                     raz_combination_group;
	EnumArray<KeyCombination, Gtk::RadioButton> raz_combination;
	Gtk::Label                                  raz_delay_label;
	Gtk::HBox                                   raz_delay_layout;
	Gtk::HScale                                 raz_delay;
	Gtk::Frame                                  pause_by_keyboard;
	Gtk::VBox                                   pause_by_keyboard_layout;
	Gtk::Label                                  pause_combination_label;
	Gtk::HBox                                   pause_combination_label_layout;
	Gtk::RadioButton::Group                     pause_combination_group;
	EnumArray<KeyModifier, Gtk::RadioButton>    pause_combination;
	Gtk::CheckButton                            pause_enabled;

	// Onglet keyboard
	Gtk::VBox                         kb_page;
	Gtk::HBox                         kb_selector_layout;
	Gtk::Label                        kb_selector_label;
	KbSelectorModel                   kb_selector_model;
	Glib::RefPtr<Gtk::ListStore>      kb_selector_data;
	Gtk::ComboBox                     kb_selector;
	Gtk::CheckButton                  display_kp;
	Gtk::Frame                        kbm_widget_frame;
	Gtk::VBox                         kbm_widget_layout;
	KeyboardMapWidget                 kbm_widget;
	Gtk::RadioButton::Group           area_selector_group;
	EnumArray<Side, Gtk::RadioButton> area_selector;
	Gtk::CheckButton                  use_mouse_buttons;

	// Onglet display
	Gtk::VBox        disp_page;
	Gtk::CheckButton display_status_bar          ;
	Gtk::CheckButton display_time_after_flag_down;
	Gtk::CheckButton display_bronstein_extra_time;
	Gtk::CheckButton display_byo_yomi_extra_time ;
};

#endif

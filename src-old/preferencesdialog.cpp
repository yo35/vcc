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


#include "preferencesdialog.h"
#include "params.h"
#include <gtkmm/stock.h>
#include <translation.h>

// Modèle pour le combo box de sélection du clavier
Gtk::TreeModelColumn<std::string               > PreferencesDialog::KbSelectorModel::code() const { return m_code; }
Gtk::TreeModelColumn<Glib::ustring             > PreferencesDialog::KbSelectorModel::name() const { return m_name; }
Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > PreferencesDialog::KbSelectorModel::icon() const { return m_icon; }
PreferencesDialog::KbSelectorModel::KbSelectorModel() : Gtk::TreeModelColumnRecord() {
	add(m_code);
	add(m_name);
	add(m_icon);
}

// Constructeur
PreferencesDialog::PreferencesDialog(Gtk::Window &parent) :
	Gtk::Dialog(_("Preferences"), parent, true, true)
{
	// Divers
	curr_kb_code = "";
	set_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

	// Boutons de réponse
	add_button(Gtk::Stock::OK    , Gtk::RESPONSE_OK    );
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	set_default_response(Gtk::RESPONSE_OK);

	// Frame "RAZ par toolbar"
	raz_by_toolbar.set_label(_("Reset through the toolbar"));
	raz_by_toolbar_layout.set_border_width(5);
	raz_by_toolbar_layout.set_spacing(5);
	ask_before_raz_label.set_label(_("Ask for confirmation before resetting"));
	ask_before_raz_label_layout.pack_start(ask_before_raz_label, Gtk::PACK_SHRINK);
	raz_by_toolbar_layout.pack_start(ask_before_raz_label_layout);
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
	raz_combination_label.set_label(_("Keys to press to reset"));
	raz_combination_label_layout.pack_start(raz_combination_label, Gtk::PACK_SHRINK);
	raz_by_keyboard_layout.pack_start(raz_combination_label_layout);
	raz_combination[DOUBLE_CTRL].set_label(_("CTRL keys (left and right)"));
	raz_combination[DOUBLE_MAJ ].set_label(_("MAJ keys (left and right)" ));
	for(KeyCombination::iterator k=KeyCombination::first(); k.valid(); ++k) {
		raz_combination[*k].set_group(raz_combination_group);
		raz_by_keyboard_layout.pack_start(raz_combination[*k]);
	}
	raz_delay_label.set_label(_("Delay (milliseconds)"));
	raz_delay_layout.set_spacing(5);
	raz_delay.set_digits(0);
	raz_delay.set_range(0, 2000);
	raz_delay.set_increments(100, 500);
	raz_delay_layout.pack_start(raz_delay_label, Gtk::PACK_SHRINK);
	raz_delay_layout.pack_start(raz_delay);
	raz_by_keyboard_layout.pack_start(raz_delay_layout);
	raz_by_keyboard.add(raz_by_keyboard_layout);

	// Frame "Pause par clavier"
	pause_by_keyboard.set_label(_("Pause through the keyboard"));
	pause_by_keyboard_layout.set_border_width(5);
	pause_by_keyboard_layout.set_spacing(5);
	pause_enabled.set_label(_("Allow the game to be paused through the keyboard"));
	pause_enabled.signal_clicked().connect(sigc::mem_fun(*this, &PreferencesDialog::on_pause_enabled_clicked));
	on_pause_enabled_clicked();
	pause_by_keyboard_layout.pack_start(pause_enabled);
	pause_combination_label.set_label(_("Keys to press to pause"));
	pause_combination_label_layout.pack_start(pause_combination_label, Gtk::PACK_SHRINK);
	pause_by_keyboard_layout.pack_start(pause_combination_label_layout);
	pause_combination[MODIFIER_VOID].set_label(_("Only the space key"));
	pause_combination[MODIFIER_CTRL].set_label(_("CTRL keys (left and right) + space"));
	pause_combination[MODIFIER_MAJ ].set_label(_("MAJ keys (left and right) + space" ));
	for(KeyModifier::iterator k=KeyModifier::first(); k.valid(); ++k) {
		pause_combination[*k].set_group(pause_combination_group);
		pause_by_keyboard_layout.pack_start(pause_combination[*k]);
	}
	pause_by_keyboard.add(pause_by_keyboard_layout);

	// Onglet RAZ
	raz_page.set_spacing(5);
	raz_page.pack_start(raz_by_toolbar   , Gtk::PACK_SHRINK);
	raz_page.pack_start(raz_by_keyboard  , Gtk::PACK_SHRINK);
	raz_page.pack_start(pause_by_keyboard, Gtk::PACK_SHRINK);

	// Onglet displaying
	display_status_bar          .set_label(_("Display the status bar at the bottom of the main window"      ));
	display_time_after_flag_down.set_label(_("Display an increasing time counter when the flag is down"     ));
	display_bronstein_extra_time.set_label(_("Display extra time information when playing in Bronstein mode"));
	display_byo_yomi_extra_time .set_label(_("Display extra time information when playing in byo-yomi mode" ));
	disp_page.set_spacing(5);
	disp_page.pack_start(display_status_bar          , Gtk::PACK_SHRINK);
	disp_page.pack_start(display_time_after_flag_down, Gtk::PACK_SHRINK);
	disp_page.pack_start(display_bronstein_extra_time, Gtk::PACK_SHRINK);
	disp_page.pack_start(display_byo_yomi_extra_time , Gtk::PACK_SHRINK);

	// Onglet keyboard (sauf géométrie)
	kb_selector_label.set_label(_("Select the keyboard layout you are using"));
	use_mouse_buttons.set_label(_("Use the mouse buttons to switch between players"));
	use_mouse_buttons.set_tooltip_text(_(
		"The left player must click on the left button, the right player on the right button"
	));
	kb_selector_data = Gtk::ListStore::create(kb_selector_model);
	kb_selector_data->set_sort_column(kb_selector_model.name(), Gtk::SORT_ASCENDING);
	kb_selector.set_model(kb_selector_data);
	kb_selector.pack_start(kb_selector_model.icon(), false);
	kb_selector.pack_start(kb_selector_model.name());
	kb_selector.signal_changed().connect(sigc::mem_fun(*this, &PreferencesDialog::on_kb_changed));
	display_kp.set_label(_("Display the numeric keypad"));
	display_kp.signal_toggled().connect(sigc::mem_fun(*this, &PreferencesDialog::on_display_kp_changed));
	kbm_widget_frame.set_label(_("Modify the active areas by clicking on the keyboard"));
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		area_selector[*k].set_group(area_selector_group);
		area_selector[*k].signal_toggled().connect(
			sigc::mem_fun(*this, &PreferencesDialog::on_area_changed));
	}
	area_selector[LEFT ].set_label(_("Modify the left player's area" ));
	area_selector[RIGHT].set_label(_("Modify the right player's area"));
	area_selector[LEFT ].set_active(true);
	on_area_changed();

	// Tooltip
	Glib::ustring lg1 = _("Active areas that players can use to engage the clock when playing");
	Glib::ustring lg2 = _("left player's area");
	Glib::ustring lg3 = _("right player's area");
  Glib::ustring coul2 = kbm_widget.color(LEFT ).to_string();
	Glib::ustring coul3 = kbm_widget.color(RIGHT).to_string();
	Glib::ustring tooltip = "";
	tooltip += "<span weight=\"bold\">" + lg1 + "</span>\n";
	tooltip += "<span background=\"" + coul2 + "\">        </span> : " + lg2 + "\n";
	tooltip += "<span background=\"" + coul3 + "\">        </span> : " + lg3;
	kbm_widget.set_tooltip_markup(tooltip);

	// Onglet keyboard (géométrie)
	kbm_widget_layout .set_spacing(5);
	kb_page           .set_spacing(5);
	kb_selector_layout.set_spacing(5);
	kbm_widget_layout.pack_start(area_selector[LEFT ], Gtk::PACK_SHRINK);
	kbm_widget_layout.pack_start(area_selector[RIGHT], Gtk::PACK_SHRINK);
	kbm_widget_layout.pack_start(kbm_widget);
	kbm_widget_layout.set_border_width(5);
	kbm_widget_frame.add(kbm_widget_layout);
	kb_selector_layout.pack_start(kb_selector_label, Gtk::PACK_SHRINK);
	kb_selector_layout.pack_start(kb_selector);
	kb_page.pack_start(kb_selector_layout, Gtk::PACK_SHRINK);
	kb_page.pack_start(display_kp, Gtk::PACK_SHRINK);
	kb_page.pack_start(kbm_widget_frame);
	kb_page.pack_start(use_mouse_buttons, Gtk::PACK_SHRINK);

	// Géométrie générale
	kb_page.set_border_width(5);
	pages.append_page(kb_page, _("Keyboard active areas"));
	raz_page.set_border_width(5);
	pages.append_page(raz_page, _("Control options"));
	disp_page.set_border_width(5);
	pages.append_page(disp_page, _("Display options"));
	get_vbox()->set_spacing(5);
	get_vbox()->pack_start(pages);
	show_all_children();
}

// Chargement des paramètres
void PreferencesDialog::load_params() {

	// Paramètres RAZ
	ask_before_raz [gp->reinit_confirm()].set_active(true);
	raz_combination[gp->reinit_keys   ()].set_active(true);
	raz_delay.set_value(gp->reinit_delay());
	pause_combination[gp->pause_keys()].set_active(true);
	pause_enabled.set_active(gp->can_pause_by_kb());

	// Paramètres d'affichage
	display_status_bar          .set_active(gp->display_status_bar          ());
	display_time_after_flag_down.set_active(gp->display_time_after_flag_down());
	display_bronstein_extra_time.set_active(gp->display_bronstein_extra_time());
	display_byo_yomi_extra_time .set_active(gp->display_byo_yomi_extra_time ());

	// Utilisation des boutons de la souris
	use_mouse_buttons.set_active(gp->use_mouse_buttons());

	// Régions sélectionnées courantes
	std::string curr_keyboard = gp->curr_keyboard();
	areas[curr_keyboard]      = gp->kam_perso();

	// Affichage du pavé numérique
	display_kp.set_active(gp->display_num_pad());
	on_display_kp_changed();

	// Liste des claviers
	std::set<std::string> keyboards = gp->keyboards();
	for(std::set<std::string>::const_iterator it=keyboards.begin(); it!=keyboards.end(); ++it) {
		Glib::ustring             kb_name = gp->keyboard_name(*it);
		Glib::RefPtr<Gdk::Pixbuf> kb_icon = gp->keyboard_icon(*it);
		Gtk::TreeModel::iterator row = kb_selector_data->append();
		(*row)[kb_selector_model.code()] = *it;
		(*row)[kb_selector_model.name()] = kb_name;
		(*row)[kb_selector_model.icon()] = kb_icon;
		if(curr_keyboard==*it) {
			kb_selector.set_active(row);
		}
	}
}

// Enregistrement des paramètres
void PreferencesDialog::save_params() {

	// Paramètres RAZ
	for(ReinitConfirm::iterator k=ReinitConfirm::first(); k.valid(); ++k) {
		if(ask_before_raz[*k].get_active())
			gp->set_reinit_confirm(*k);
	}
	for(KeyCombination::iterator k=KeyCombination::first(); k.valid(); ++k) {
		if(raz_combination[*k].get_active())
			gp->set_reinit_key(*k);
	}
	gp->set_reinit_delay(raz_delay.get_value());
	for(KeyModifier::iterator k=KeyModifier::first(); k.valid(); ++k) {
		if(pause_combination[*k].get_active())
			gp->set_pause_key(*k);
	}
	gp->set_can_pause_by_kb(pause_enabled.get_active());

	// Paramètres d'affichage
	gp->set_display_status_bar          (display_status_bar          .get_active());
	gp->set_display_time_after_flag_down(display_time_after_flag_down.get_active());
	gp->set_display_bronstein_extra_time(display_bronstein_extra_time.get_active());
	gp->set_display_byo_yomi_extra_time (display_byo_yomi_extra_time .get_active());

	// Paramètres de clavier
	save_curr_area();
	gp->set_curr_keyboard(curr_kb_code);
	gp->set_display_num_pad(display_kp.get_active());
	gp->set_kam_perso(areas[curr_kb_code]);
	gp->set_use_mouse_buttons(use_mouse_buttons.get_active());
}

// Event handling
bool PreferencesDialog::on_key_press_event(GdkEventKey* event) {
	if(pages.get_current_page()==0) {
		kbm_widget.on_key_action(event->keyval, true);
		return true;
	}
	else
		return Gtk::Dialog::on_key_press_event(event);
}

// Event handling
bool PreferencesDialog::on_key_release_event(GdkEventKey* event) {
	if(pages.get_current_page()==0) {
		kbm_widget.on_key_action(event->keyval, false);
		return true;
	}
	else
		return Gtk::Dialog::on_key_release_event(event);
}

// Callback appelé sur un changement de clavier
void PreferencesDialog::on_kb_changed() {

	// Enregistrement de l'état des régions précédentes
	if(curr_kb_code != "")
		save_curr_area();

	// Modification du plan de clavier
	curr_kb_code = (*kb_selector.get_active())[kb_selector_model.code()];
	kbm_widget.set_keyboard_map(gp->keyboard_map(curr_kb_code));

	// Chargement du plan de région courant
	if(areas.find(curr_kb_code)==areas.end()) {
		areas[curr_kb_code] = gp->default_area_map(curr_kb_code);
	}
	kbm_widget.set_area_map(areas[curr_kb_code]);
}

// Modification du choix d'affichage du pavé numérique
void PreferencesDialog::on_display_kp_changed() {
	kbm_widget.set_display_kp(display_kp.get_active());
}

// Changement de la zone active
void PreferencesDialog::on_area_changed() {
	for(Side::iterator it=Side::first(); it.valid(); ++it) {
		if(area_selector[*it].get_active())
			kbm_widget.set_active_side(*it);
	}
}

// Enregistrement du plan de région courant
void PreferencesDialog::save_curr_area() {
	assert(curr_kb_code!="");
	areas[curr_kb_code] = kbm_widget.get_area_map();
}

// Active/désactive les options liées à la mise en pause
void PreferencesDialog::on_pause_enabled_clicked()
{
	bool is_pause_enabled = pause_enabled.get_active();
	for(KeyModifier::iterator k=KeyModifier::first(); k.valid(); ++k) {
		pause_combination[*k].set_sensitive(is_pause_enabled);
	}
}
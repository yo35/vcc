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


#include "timecontroldialog.h"
#include <gtkmm/stock.h>
#include <translation.h>

TimeControlDialog::TimeControlDialog(Gtk::Window &parent, const TimeControl &src) :
	Gtk::Dialog(_("Time control"), parent, true, true)
{
	// Boutons de réponse
	add_button(Gtk::Stock::OK    , Gtk::RESPONSE_OK    );
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	set_default_response(Gtk::RESPONSE_OK);

	// Frame "mode"
	frm_mode.set_label(_("Mode"));
	layout_mode.set_spacing(5);
	mode[0].set_label(_("Sudden death"));
	mode[1].set_label(_("Fischer"     ));
	mode[2].set_label(_("Bronstein"   ));
	mode[3].set_label(_("Hourglass"   ));
	for(int i=0; i<4; ++i) {
		mode[i].set_group(group);
		mode[i].signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::manage_sensitivity));
		layout_mode.pack_start(mode[i]);
	}
	frm_mode.add(layout_mode);

	// Frames "time"
	frm_time[LEFT ].set_label(_("Left" ));
	frm_time[RIGHT].set_label(_("Right"));
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		lbl_main_time[*k].set_label(_("Main time"));
		lbl_increment[*k].set_label(_("Increment"));
		layout_time  [*k].resize(2, 2);
		layout_time  [*k].set_spacings(5);
		layout_time  [*k].attach(lbl_main_time[*k], 0, 1, 0, 1);
		layout_time  [*k].attach(lbl_increment[*k], 0, 1, 1, 2);
		layout_time  [*k].attach(    main_time[*k], 1, 2, 0, 1);
		layout_time  [*k].attach(    increment[*k], 1, 2, 1, 2);
		frm_time     [*k].add(layout_time[*k]);
	}
	main_time[LEFT].signal_changed().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_main_time));
	increment[LEFT].signal_changed().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_increment));
	layout_times.set_spacing(5);
	layout_times.pack_start(frm_time[LEFT ]);
	layout_times.pack_start(frm_time[RIGHT]);

	// Case à cocher liant les deux côtés
	link_both_times.set_label(_("Time control values are the same for both sides"));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::manage_sensitivity));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_main_time));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_increment));

	// Géométrie générale
	get_vbox()->set_spacing(5);
	get_vbox()->pack_start(frm_mode       );
	get_vbox()->pack_start(link_both_times);
	get_vbox()->pack_start(layout_times   );
	show_all_children();

	// Initialisation des widgets
	set_time_control(src);
}

TimeControl TimeControlDialog::get_time_control() const {
	TimeControl retval;
	if(mode[0].get_active()) retval.set_mode(TimeControl::SUDDEN_DEATH);
	if(mode[1].get_active()) retval.set_mode(TimeControl::FISCHER     );
	if(mode[2].get_active()) retval.set_mode(TimeControl::BRONSTEIN   );
	if(mode[3].get_active()) retval.set_mode(TimeControl::HOURGLASS   );
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		retval.set_main_time(main_time[*k].get_time(), *k);
		if(retval.mode()==TimeControl::FISCHER || retval.mode()==TimeControl::BRONSTEIN)
			retval.set_increment(increment[*k].get_time(), *k);
	}
	return retval;
}

void TimeControlDialog::set_time_control(const TimeControl &src) {
	mode[src.mode()].set_active(true);
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		main_time[*k].set_time(src.main_time(*k));
		if(src.mode()==TimeControl::FISCHER || src.mode()==TimeControl::BRONSTEIN) {
			increment[*k].set_time(src.increment(*k));
		}
		else {
			increment[*k].set_time(0);
		}
	}
	link_both_times.set_active(src.both_sides_have_same_time());
	manage_sensitivity();
}

void TimeControlDialog::manage_sensitivity() {
	bool enable_increment = (mode[1].get_active() || mode[2].get_active());
	bool enable_right     = !link_both_times.get_active();
	increment[LEFT ].set_sensitive(enable_increment);
	increment[RIGHT].set_sensitive(enable_increment && enable_right);
	main_time[RIGHT].set_sensitive(                    enable_right);
}

void TimeControlDialog::copy_left_main_time() {
	if(link_both_times.get_active())
		main_time[RIGHT].set_time(main_time[LEFT].get_time());
}

void TimeControlDialog::copy_left_increment() {
	if(link_both_times.get_active())
		increment[RIGHT].set_time(increment[LEFT].get_time());
}

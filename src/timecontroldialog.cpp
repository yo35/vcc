/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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

	// Description des cadences
	EnumArray<TimeControlType, Glib::ustring> mode_description;
	mode_description[SUDDEN_DEATH] = _(
		"Simplest time control mode. No additional delay is never added to the "
		"player's main time."
	);
	mode_description[FISCHER] = _(
		"At each move, the player is granted an additional delay, so that his or "
		"her global clock time is always greater than this bonus time at the "
		"beginning of his or her playing turn. Additional delays can be "
		"accumulated."
	);
	mode_description[BRONSTEIN] = _(
		"Same as Fischer time control mode, except that additional delays cannot "
		"be accumulated. Bronstein mode can also be thought as a kind of sudden "
		"death mode in which the clock waits for a certain delay before starting "
		"to substract from the player's remaining time when engaged."
	);
	mode_description[HOURGLASS] = _(
		"While the current player is deciding on his or her move, the opponent's "
		"time is increasing. Therefore, the sum of both clocks remains always the "
		"same."
	);
	mode_description[BYO_YOMI] = _(
		"When the player's main time expires, he or she is granted one or more "
		"additional time periods, denoted as \"byo-yomi\" periods. If the move is "
		"completed before the current byo-yomi period expires, the clock is reset "
		"with the whole byo-yomi time available for the next move; otherwise, the "
		"next byo-yomi period starts. At the end of the last byo-yomi period, the "
		"game is lost."
	);

	// Frame "mode"
	frm_mode.set_label(_("Mode"));
	layout_mode.set_border_width(5);
	layout_mode.set_spacing(5);
	for(TimeControlType::iterator k=TimeControlType::first(); k.valid(); ++k) {
		Glib::ustring mode_name = time_control_type_name(*k);
		Glib::ustring tooltip   = Glib::ustring::compose("<span weight=\"bold\">%1</span>\n%2",
			mode_name, mode_description[*k]);
		mode[*k].set_label(mode_name);
		mode[*k].set_tooltip_markup(tooltip);
		mode[*k].set_group(mode_group);
		mode[*k].signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::manage_sensitivity));
		layout_mode.pack_start(mode[*k]);
	}
	frm_mode.add(layout_mode);

	// Frames "time"
	frm_time[LEFT ].set_label(_("Left" ));
	frm_time[RIGHT].set_label(_("Right"));
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		byo_period    [*k].set_range(1, 99);
		byo_period    [*k].set_increments(1, 5);
		lbl_main_time [*k].set_label(_("Main time"));
		lbl_increment [*k].set_label(_("Increment"));
		lbl_byo_period[*k].set_label(_("Byo-yomi periods"));
		layout_time   [*k].resize(2, 3);
		layout_time   [*k].set_border_width(5);
		layout_time   [*k].set_spacings(5);
		layout_time   [*k].attach(lbl_main_time [*k], 0, 1, 0, 1);
		layout_time   [*k].attach(lbl_increment [*k], 0, 1, 1, 2);
		layout_time   [*k].attach(lbl_byo_period[*k], 0, 1, 2, 3);
		layout_time   [*k].attach(    main_time [*k], 1, 2, 0, 1);
		layout_time   [*k].attach(    increment [*k], 1, 2, 1, 2);
		layout_time   [*k].attach(    byo_period[*k], 1, 2, 2, 3);
		frm_time      [*k].add(layout_time[*k]);
	}
	main_time [LEFT].signal_changed      ().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_main_time ));
	increment [LEFT].signal_changed      ().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_increment ));
	byo_period[LEFT].signal_value_changed().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_byo_period));
	layout_times.set_spacing(5);
	layout_times.pack_start(frm_time[LEFT ]);
	layout_times.pack_start(frm_time[RIGHT]);

	// Case à cocher liant les deux côtés
	link_both_times.set_label(_("Time control values are the same for both sides"));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::manage_sensitivity));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_main_time ));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_increment ));
	link_both_times.signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_byo_period));

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
	for(TimeControlType::iterator k=TimeControlType::first(); k.valid(); ++k) {
		if(mode[*k].get_active())
			retval.set_mode(*k);
	}
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		retval.set_main_time(main_time[*k].get_time(), *k);
		if(retval.mode()==FISCHER || retval.mode()==BRONSTEIN || retval.mode()==BYO_YOMI) {
			retval.set_increment(increment[*k].get_time(), *k);
		}
		if(retval.mode()==BYO_YOMI) {
			retval.set_byo_period(byo_period[*k].get_value_as_int(), *k);
		}
	}
	return retval;
}

void TimeControlDialog::set_time_control(const TimeControl &src) {
	mode[src.mode()].set_active(true);
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		main_time[*k].set_time(src.main_time(*k));
		if(src.mode()==FISCHER || src.mode()==BRONSTEIN || src.mode()==BYO_YOMI) {
			increment[*k].set_time(src.increment(*k));
		}
		else {
			increment[*k].set_time(0);
		}
		if(src.mode()==BYO_YOMI) {
			byo_period[*k].set_value(src.byo_period(*k));
		}
		else {
			byo_period[*k].set_value(1);
		}
	}
	link_both_times.set_active(src.both_sides_have_same_time());
	manage_sensitivity();
}

void TimeControlDialog::manage_sensitivity() {
	bool enable_increment  = (mode[FISCHER].get_active() || mode[BRONSTEIN].get_active() || mode[BYO_YOMI].get_active());
	bool enable_byo_period = mode[BYO_YOMI].get_active();
	bool enable_right      = !link_both_times.get_active();
	increment [LEFT ].set_sensitive(enable_increment );
	byo_period[LEFT ].set_sensitive(enable_byo_period);
	main_time [RIGHT].set_sensitive(                     enable_right);
	increment [RIGHT].set_sensitive(enable_increment  && enable_right);
	byo_period[RIGHT].set_sensitive(enable_byo_period && enable_right);
}

void TimeControlDialog::copy_left_main_time() {
	if(link_both_times.get_active()) {
		main_time[RIGHT].set_time(main_time[LEFT].get_time());
	}
}

void TimeControlDialog::copy_left_increment() {
	if(link_both_times.get_active()) {
		increment[RIGHT].set_time(increment[LEFT].get_time());
	}
}

void TimeControlDialog::copy_left_byo_period() {
	if(link_both_times.get_active()) {
		byo_period[RIGHT].set_value(byo_period[LEFT].get_value_as_int());
	}
}

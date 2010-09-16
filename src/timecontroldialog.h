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


#ifndef TIMECONTROLDIALOG_H
#define TIMECONTROLDIALOG_H

#include <gtkmm/dialog.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include "timeentry.h"
#include "timecontrol.h"

class TimeControlDialog : public Gtk::Dialog {

public:

	// Constructeur
	TimeControlDialog(Gtk::Window &parent, const TimeControl &src);

	// Accesseur/modifieur
	void set_time_control(const TimeControl &src);
	TimeControl get_time_control() const;

private:

	// Comportement interne de la boîte de dialogue
	void manage_sensitivity();
	void copy_left_main_time();
	void copy_left_increment();

	// Widget
	Gtk::CheckButton        link_both_times;
	Gtk::RadioButton        mode[4];
	Gtk::RadioButton::Group group;
	TimeEntry               main_time[2];
	TimeEntry               increment[2];
	Gtk::Label              lbl_main_time[2];
	Gtk::Label              lbl_increment[2];
	Gtk::Frame              frm_time[2];
	Gtk::Frame              frm_mode;

	// Layout
	Gtk::VBox  layout_mode;
	Gtk::Table layout_time[2];
	Gtk::HBox  layout_times;
};

#endif

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


#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/separatortoolitem.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/image.h>
#include "icon.h"
#include "dialwidget.h"
#include "bitimer.h"
#include "eventdelayer.h"
#include "keys.h"
#include "keyboardmap.h"
#include "areamap.h"

class ClockWindow : public Gtk::Window {

public:
	ClockWindow();

protected:

	// Event handling
	virtual bool on_key_press_event  (GdkEventKey* event);
	virtual bool on_key_release_event(GdkEventKey* event);

private:

	// Event handling
	void on_myself_shown();
	void on_pause_clicked();
	void on_reset_clicked();
	void on_tctrl_clicked();
	void on_prefs_clicked();
	void on_help_clicked ();
	void on_about_clicked();
	void on_clock_button_clicked(const Side &side);
	void on_reset_triggered_from_kb();
	void on_debug_delayer_elapsed();
	void retrieve_parameters_from_gp();

	// Données membres
	BiTimer            core;
	EventDelayer       debug_delayer;
	EventDelayer       reinit_delayer;
	Keyval             reinit_trigger[2];
	const KeyboardMap *curr_kbm;
	const AreaMap     *curr_kam;
	Keycode            curr_key_down;

	// Widgets
	EnumArray<Side, DialWidget> dial;
	Gtk::Toolbar                toolbar;
	Icon                        ico_reset;
	Icon                        ico_pause;
	Icon                        ico_tctrl;
	Gtk::Image                  img_reset;
	Gtk::Image                  img_pause;
	Gtk::Image                  img_tctrl;
	Gtk::ToolButton             btn_reset;
	Gtk::ToolButton             btn_pause;
	Gtk::ToolButton             btn_tctrl;
	Gtk::ToolButton             btn_prefs;
	Gtk::ToolButton             btn_help ;
	Gtk::ToolButton             btn_about;
	Gtk::SeparatorToolItem      sep_toolbar;
	Gtk::HBox                   dial_layout;
	Gtk::VBox                   main_layout;
};

#endif

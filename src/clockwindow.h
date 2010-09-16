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


#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/image.h>
#include "dialwidget.h"

class ClockWindow : public Gtk::Window {

public:
	ClockWindow();

protected:

	// Event handling
	virtual bool on_key_press_event(GdkEventKey* event);

private:

	// Event handling
	void on_pause_clicked();
	void on_reset_clicked();
	void on_tctrl_clicked();
	void on_clock_button_clicked(int no);

	// Modifie le cadran actif
	void start_timer(int no);
	void change_timer();
	void stop_timer();

	// Remets les timers à zéro
	void reset_timers();

	// Test si l'un des timers est actif
	bool one_timer_is_active() const;

	int             bronstein_limit[2];
	int             no_actif;
	Timer           timer[2];
	DialWidget      dial [2];
	Gtk::Toolbar    toolbar;
	Gtk::Image      img_reset;
	Gtk::Image      img_pause;
	Gtk::Image      img_tctrl;
	Gtk::ToolButton btn_reset;
	Gtk::ToolButton btn_pause;
	Gtk::ToolButton btn_tctrl;
	Gtk::HBox       dial_layout;
	Gtk::VBox       main_layout;
};

#endif

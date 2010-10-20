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


#include "clockwindow.h"
#include "vccaboutdialog.h"
#include "timecontroldialog.h"
#include "keys.h"
#include "params.h"
#include <config.h>
#include <cassert>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <translation.h>

#ifdef OS_IS_WINDOWS
	#include <winkeyhookdll.h>
#endif

ClockWindow::ClockWindow() : Gtk::Window(),
	ico_reset(gp->prefix_path() + "/" + VCC_SHARE_RPATH + "/reset.png"),
	ico_pause(gp->prefix_path() + "/" + VCC_SHARE_RPATH + "/pause.png"),
	ico_tctrl(gp->prefix_path() + "/" + VCC_SHARE_RPATH + "/tctrl.png")
{

	// Divers
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);
	set_title(PROJECT_FULL_NAME " " PROJECT_VERSION_MAJOR "." PROJECT_VERSION_MINOR);

	// Désactivation de la touche windows
	#ifdef OS_IS_WINDOWS
		set_kbd_hook();
	#endif

	// Configuration des cadrans
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		dial[*k].set_can_focus(true);
		dial[*k].set_timer(core.timer(*k));
	}

	// Gestion des événements
	btn_reset.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_reset_clicked));
	btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_pause_clicked));
	btn_tctrl.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_tctrl_clicked));
	btn_about.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_about_clicked));

	// Toolbar
	int icon_height = 0;
	int icon_width  = 0;
	Gtk::IconSize::lookup(toolbar.get_icon_size(), icon_width, icon_height);
	img_reset.set(ico_reset.get(icon_height));
	img_pause.set(ico_pause.get(icon_height));
	img_tctrl.set(ico_tctrl.get(icon_height));
	btn_reset.set_icon_widget(img_reset);
	btn_pause.set_icon_widget(img_pause);
	btn_tctrl.set_icon_widget(img_tctrl);
	btn_reset.set_label(_("Reset"       ));
	btn_pause.set_label(_("Pause"       ));
	btn_tctrl.set_label(_("Time control"));
	btn_reset.set_tooltip_text(_("Reset the clock"                ));
	btn_pause.set_tooltip_text(_("Pause the clock"                ));
	btn_tctrl.set_tooltip_text(_("Change the current time control"));
	btn_about.set_stock_id(Gtk::Stock::ABOUT);
	toolbar.append(btn_reset  );
	toolbar.append(btn_pause  );
	toolbar.append(btn_tctrl  );
	toolbar.append(btn_about  );

	// Géométrie générale
	dial_layout.pack_start(dial[LEFT ]);
	dial_layout.pack_start(dial[RIGHT]);
	main_layout.pack_start(toolbar, Gtk::PACK_SHRINK);
	main_layout.pack_start(dial_layout);
	main_layout.set_spacing(5);
	add(main_layout);
	show_all_children();
	dial[LEFT].grab_focus();
}

bool ClockWindow::on_key_press_event(GdkEventKey* event)  {

	// Décodage
	Keycode code = event->hardware_keycode;
	BoolSideArray test_area;
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		test_area[*k] = (gp->key_area[*k].find(code) != gp->key_area[*k].end());
	}

	// On appelle la fonction qvb
	assert(!(test_area[LEFT] && test_area[RIGHT]));
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		if(test_area[*k]) {
			on_clock_button_clicked(*k);
		}
	}
	return true;
}

void ClockWindow::on_pause_clicked() {
	if(core.mode()==BiTimer::ACTIVE)
		core.stop_timer();
}

void ClockWindow::on_reset_clicked() {

	// On demande confirmation si l'un des timers est actif
	if(core.mode()==BiTimer::ACTIVE) {
		Gtk::MessageDialog dialog(*this, _("Do you really want to start a new game?"),
			false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
		dialog.set_title(_("Stop this game?"));
		int retval = dialog.run();
		if(retval!=Gtk::RESPONSE_YES)
			return;
	}

	// Remise à zéro proprement dite
	core.reset_timers();
}

void ClockWindow::on_tctrl_clicked() {
	TimeControlDialog dialog(*this, core.time_control());
	int retval = dialog.run();
	if(retval!=Gtk::RESPONSE_OK)
		return;
	core.set_time_control(dialog.get_time_control());
}

void ClockWindow::on_about_clicked() {
	VccAboutDialog dialog;
	dialog.run();
}

void ClockWindow::on_clock_button_clicked(const Side &side) {
	if(core.mode()==BiTimer::ACTIVE) {
		if(core.active_side()==side)
			core.change_timer();
	}
	else {
		core.start_timer(rev(side));
	}
}

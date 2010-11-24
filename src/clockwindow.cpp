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
#include "preferencesdialog.h"
#include "debugdialog.h"
#include "params.h"
#include <config.h>
#include <translation.h>
#include <cassert>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <gdk/gdkkeysyms.h>

#ifdef OS_IS_WINDOWS
	#include <winkeyhookdll.h>
#endif

ClockWindow::ClockWindow() : Gtk::Window(), debug_delayer(3), reinit_delayer(2),
	ico_reset(gp->prefix_path() + "/" VCC_SHARE_RPATH "/reset.png"),
	ico_pause(gp->prefix_path() + "/" VCC_SHARE_RPATH "/pause.png"),
	ico_tctrl(gp->prefix_path() + "/" VCC_SHARE_RPATH "/tctrl.png")
{
	// Initialisation de la pendule
	core.set_time_control(gp->initial_time_control());

	// Initialisation du retardateur pour l'affichage de la fenêtre de débug
	#ifdef DEV_COMPILATION
		debug_delayer.set_delay(750);
	#else
		debug_delayer.set_delay(5000);
	#endif
	debug_delayer.signal_occurred().connect(
		sigc::mem_fun(*this, &ClockWindow::on_debug_delayer_elapsed));

	// Initialisation des objets de gestion de clavier
	reinit_delayer.signal_occurred().connect(
		sigc::mem_fun(*this, &ClockWindow::on_reset_triggered_from_kb));
	retrieve_parameters_from_gp();

	// Divers
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);
	set_title(PROJECT_FULL_NAME " " PROJECT_VERSION_MAJOR "." PROJECT_VERSION_MINOR);
	signal_show().connect(sigc::mem_fun(*this, &ClockWindow::on_myself_shown));

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
	btn_prefs.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_prefs_clicked));
	btn_about.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_about_clicked));

	// Toolbar
	//sep_toolbar.set_draw(false);
	//sep_toolbar.set_expand(true);
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
	btn_prefs.set_stock_id(Gtk::Stock::PREFERENCES);
	btn_about.set_stock_id(Gtk::Stock::ABOUT      );
	btn_reset.set_tooltip_text(_("Reset the clock"                      ));
	btn_pause.set_tooltip_text(_("Pause the clock"                      ));
	btn_tctrl.set_tooltip_text(_("Change the current time control"      ));
	btn_prefs.set_tooltip_text(_("Configuration"                        ));
	btn_about.set_tooltip_text(_("Information about credits and license"));
	toolbar.append(btn_reset  );
	toolbar.append(btn_pause  );
	toolbar.append(sep_toolbar);
	toolbar.append(btn_tctrl  );
	toolbar.append(btn_prefs  );
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

bool ClockWindow::on_key_press_event(GdkEventKey* event) {

	// Réinitialisation par le clavier
	if(event->keyval==reinit_trigger[0]) reinit_delayer.trigger(0);
	if(event->keyval==reinit_trigger[1]) reinit_delayer.trigger(1);

	// Fenêtre de débug
	if(event->keyval==GDK_d || event->keyval==GDK_D) debug_delayer.trigger(0);
	if(event->keyval==GDK_b || event->keyval==GDK_B) debug_delayer.trigger(1);
	if(event->keyval==GDK_g || event->keyval==GDK_G) debug_delayer.trigger(2);

	// Détection de zone
	assert(curr_kbm!=0 && curr_kam!=0);
	int physical_key = curr_kbm->get_key(event->keyval);
	if(physical_key>=0) {
		assert(physical_key<curr_kam->nb_keys());
		if(curr_kam->is_affected(physical_key)) {
			on_clock_button_clicked(curr_kam->side(physical_key));
		}
	}
	return true;
}

bool ClockWindow::on_key_release_event(GdkEventKey* event) {

	// Réinitialisation par le clavier
	if(event->keyval==reinit_trigger[0]) reinit_delayer.cancel_trigger(0);
	if(event->keyval==reinit_trigger[1]) reinit_delayer.cancel_trigger(1);

	// Fenêtre de débug
	if(event->keyval==GDK_d || event->keyval==GDK_D) debug_delayer.cancel_trigger(0);
	if(event->keyval==GDK_b || event->keyval==GDK_B) debug_delayer.cancel_trigger(1);
	if(event->keyval==GDK_g || event->keyval==GDK_G) debug_delayer.cancel_trigger(2);
	return true;
}

void ClockWindow::on_myself_shown() {
	if(!gp->first_launch())
		return;
	PreferencesDialog dialog(*this, true);
	dialog.load_params();
	dialog.run();
	dialog.save_params();
	gp->set_first_launch(false);
	retrieve_parameters_from_gp();
}

void ClockWindow::on_pause_clicked() {
	if(core.mode()==BiTimer::ACTIVE)
		core.stop_timer();
}

void ClockWindow::on_reset_clicked() {

	// Lecture du paramètre relatif à la demande de confirmation
	ReinitConfirm confirm_mode = gp->reinit_confirm();

	// On demande confirmation suivant la valeur du paramètre de demande de confirmation
	if(confirm_mode==RC_ALWAYS ||
		(confirm_mode==RC_IF_NOT_PAUSED && core.mode()==BiTimer::ACTIVE))
	{
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
	gp->set_initial_time_control(core.time_control());
}

void ClockWindow::on_prefs_clicked() {
	PreferencesDialog dialog(*this);
	dialog.load_params();
	int retval = dialog.run();
	if(retval!=Gtk::RESPONSE_OK)
		return;
	dialog.save_params();
	retrieve_parameters_from_gp();
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

void ClockWindow::on_reset_triggered_from_kb() {
	core.reset_timers();
}

void ClockWindow::on_debug_delayer_elapsed() {
	DebugDialog dialog(*this);
	dialog.run();
}

void ClockWindow::retrieve_parameters_from_gp() {
	reinit_delayer.set_delay(gp->reinit_delay());
	curr_kbm = &gp->keyboard_map(gp->curr_keyboard());
	curr_kam = &gp->kam_perso();
	KeyCombination kc = gp->reinit_keys();
	if(kc==DOUBLE_CTRL) {
		reinit_trigger[0] = GDK_Control_L;
		reinit_trigger[1] = GDK_Control_R;
	}
	else if(kc==DOUBLE_MAJ) {
		reinit_trigger[0] = GDK_Shift_L;
		reinit_trigger[1] = GDK_Shift_R;
	}
	else
		assert(false);
}

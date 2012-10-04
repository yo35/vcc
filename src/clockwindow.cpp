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


#include "clockwindow.h"
#include "vccaboutdialog.h"
#include "timecontroldialog.h"
#include "namedialog.h"
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

ClockWindow::ClockWindow() : Gtk::Window(), debug_delayer(3), reinit_delayer(2), pause_delayer(3),
	ico_reset(gp->prefix_path() + "/" VCC_SHARE_RPATH "/reset.png"),
	ico_pause(gp->prefix_path() + "/" VCC_SHARE_RPATH "/pause.png"),
	ico_tctrl(gp->prefix_path() + "/" VCC_SHARE_RPATH "/tctrl.png"),
	ico_names(gp->prefix_path() + "/" VCC_SHARE_RPATH "/names.png")
{
	// Initialisation de la pendule
	core.set_time_control(gp->initial_time_control());
	statusbar.push(core.time_control().description());

	// Initialisation des noms des joueurs
	display_players_names = gp->visible_names();
	for(Side::iterator side=Side::first(); side.valid(); ++side) {
		dial[*side].set_display_label(display_players_names);
		dial[*side].set_label(gp->player_name(*side));
	}

	// Initialisation du retardateur pour l'affichage de la fenêtre de débug
	#ifdef DEV_COMPILATION
		debug_delayer.set_delay(750);
	#else
		debug_delayer.set_delay(5000);
	#endif
	debug_delayer.signal_occurred().connect(
		sigc::mem_fun(*this, &ClockWindow::on_debug_delayer_elapsed));

	// Initialisation des objets de gestion de clavier
	reinit_delayer.signal_occurred().connect(sigc::mem_fun(*this, &ClockWindow::on_reset_triggered_from_kb));
	pause_delayer .signal_occurred().connect(sigc::mem_fun(*this, &ClockWindow::on_pause_triggered_from_kb));
	curr_key_down     = 0;
	use_mouse_buttons = false;
	can_pause_by_kb   = true ;

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
		dial[*k].set_timer(core, *k);
	}

	// Gestion des événements
	btn_reset.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_reset_clicked));
	btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_pause_clicked));
	btn_tctrl.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_tctrl_clicked));
	btn_names.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_names_clicked));
	btn_prefs.signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_prefs_clicked));
	btn_help .signal_clicked().connect(sigc::mem_fun(*this, &ClockWindow::on_help_clicked ));
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
	img_names.set(ico_names.get(icon_height));
	btn_reset.set_icon_widget(img_reset);
	btn_pause.set_icon_widget(img_pause);
	btn_tctrl.set_icon_widget(img_tctrl);
	btn_names.set_icon_widget(img_names);
	btn_reset.set_label(_("Reset"         ));
	btn_pause.set_label(_("Pause"         ));
	btn_tctrl.set_label(_("Time control"  ));
	btn_tctrl.set_label(_("Players' names"));
	btn_prefs.set_stock_id(Gtk::Stock::PREFERENCES);
	btn_help .set_stock_id(Gtk::Stock::HELP       );
	btn_about.set_stock_id(Gtk::Stock::ABOUT      );
	btn_reset.set_tooltip_text(_("Reset the clock"                          ));
	btn_pause.set_tooltip_text(_("Pause the clock"                          ));
	btn_tctrl.set_tooltip_text(_("Change the current time control"          ));
	btn_names.set_tooltip_text(_("Edit the names of the players"            ));
	btn_prefs.set_tooltip_text(_("Set the configuration and the preferences"));
	btn_help .set_tooltip_text(_("A short help text"                        ));
	btn_about.set_tooltip_text(_("Information about credits and license"    ));
	toolbar.append(btn_reset);
	toolbar.append(btn_pause);
	toolbar.append(sep1     );
	toolbar.append(btn_tctrl);
	toolbar.append(btn_names);
	toolbar.append(sep2     );
	toolbar.append(btn_prefs);
	toolbar.append(btn_help );
	toolbar.append(btn_about);

	// Géométrie générale
	dial_layout.pack_start(dial[LEFT ]);
	dial_layout.pack_start(dial[RIGHT]);
	main_layout.pack_start(toolbar, Gtk::PACK_SHRINK);
	main_layout.pack_start(dial_layout);
	main_layout.pack_start(statusbar, Gtk::PACK_SHRINK);
	main_layout.set_spacing(5);
	add(main_layout);
	show_all_children();
	dial[LEFT].grab_focus();
}

bool ClockWindow::on_key_press_event(GdkEventKey* event) {

	// Coupe-circuit pour gérer les problèmes d'événements multiples
	// lorsque l'on maintient une touche enfoncée
	if(curr_key_down==event->hardware_keycode) {
		return true;
	}
	curr_key_down = event->hardware_keycode;

	// Réinitialisation par le clavier
	if(event->keyval==reinit_trigger[0]) reinit_delayer.trigger(0);
	if(event->keyval==reinit_trigger[1]) reinit_delayer.trigger(1);

	// Pause par le clavier
	if(event->keyval==pause_trigger[0]) pause_delayer.trigger(0);
	if(event->keyval==pause_trigger[1]) pause_delayer.trigger(1);
	if(event->keyval==pause_trigger[2]) pause_delayer.trigger(2);

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

	// Coupe-circuit pour gérer les problèmes d'événements multiples
	// lorsque l'on maintient une touche enfoncée
	if(curr_key_down==event->hardware_keycode) {
		curr_key_down = 0;
	}

	// Réinitialisation par le clavier
	if(event->keyval==reinit_trigger[0]) reinit_delayer.cancel_trigger(0);
	if(event->keyval==reinit_trigger[1]) reinit_delayer.cancel_trigger(1);

	// Pause par le clavier
	if(event->keyval==pause_trigger[0]) pause_delayer.cancel_trigger(0);
	if(event->keyval==pause_trigger[1]) pause_delayer.cancel_trigger(1);
	if(event->keyval==pause_trigger[2]) pause_delayer.cancel_trigger(2);

	// Fenêtre de débug
	if(event->keyval==GDK_d || event->keyval==GDK_D) debug_delayer.cancel_trigger(0);
	if(event->keyval==GDK_b || event->keyval==GDK_B) debug_delayer.cancel_trigger(1);
	if(event->keyval==GDK_g || event->keyval==GDK_G) debug_delayer.cancel_trigger(2);
	return true;
}

bool ClockWindow::on_button_press_event(GdkEventButton *event)
{
	if(use_mouse_buttons) {
		if(event->button==1) on_clock_button_clicked(LEFT );
		if(event->button==3) on_clock_button_clicked(RIGHT);
	}
	return Gtk::Window::on_button_press_event(event);
}

void ClockWindow::on_myself_shown() {

	// Récupération des paramètres divers
	retrieve_parameters_from_gp();

	// Détection premier démarrage
	if(!gp->first_launch())
		return;
	gp->set_first_launch(false);

	// Enregistrement du clavier par défaut
	// Rq : la valeur par défaut renvoyée par 'gp->curr_keyboard()' dépend de
	// la locale. Donc, il faut absolument écrire dans vcc.ini cette valeur par
	// défaut au premier lancement afin que le modèle de clavier (fichier .kbm)
	// soit cohérent avec le fichier de description de zones (fichier .kam)
	// correspondant
	std::string curr_keyboard = gp->curr_keyboard();
	gp->set_curr_keyboard(curr_keyboard);

}

void ClockWindow::on_pause_clicked() {
	if(core.mode()==BiTimer::ACTIVE)
		core.stop_timer();
}

void ClockWindow::on_reset_clicked() {

	// On demande confirmation suivant la valeur du paramètre de demande de confirmation
	if(reinit_confirm_mode==RC_ALWAYS ||
		(reinit_confirm_mode==RC_IF_NOT_PAUSED && core.mode()==BiTimer::ACTIVE))
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
	statusbar.pop();
	statusbar.push(core.time_control().description());
	gp->set_initial_time_control(core.time_control());
}

void ClockWindow::on_names_clicked()
{
	NameDialog dialog(*this);
	dialog.set_visible_names(display_players_names);
	for(Side::iterator side=Side::first(); side.valid(); ++side) {
		dialog.set_name(*side, dial[*side].label());
	}
	if(dialog.run()!=Gtk::RESPONSE_OK) {
		return;
	}
	display_players_names = dialog.visible_names();
	gp->set_visible_names(display_players_names);
	for(Side::iterator side=Side::first(); side.valid(); ++side) {
		dial[*side].set_display_label(display_players_names);
		dial[*side].set_label(dialog.name(*side));
		gp->set_player_name(*side, dial[*side].label());
	}
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

void ClockWindow::on_help_clicked() {
	Gtk::MessageDialog dialog(*this, "", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
	dialog.set_title  (_("Help"));
	dialog.set_message(_(
		"To start playing and then change the running side of the clock, just push "
		"some keys on the keyboard. The left player have to push some key on the left "
		"side of the keyboard, the right player on the right side.\n"
		"\n"
		"If it does not work, it might be due to a misdetection of your keyboard "
		"layout; you can check it and correct it through the \"Preferences\" dialog. "
		"If your actual keyboard layout is not available among those proposed by "
		"VCC, you should select \"Default QWERTY\" or one of the default layouts that "
		"suits good to your keyboard. Thus, only the A-Z keys will be used by VCC, "
		"but the software will be usable anyway."
	));
	dialog.run();
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

void ClockWindow::on_pause_triggered_from_kb() {
	if(can_pause_by_kb) {
		on_pause_clicked();
	}
}

void ClockWindow::on_debug_delayer_elapsed() {
	DebugDialog dialog(*this);
	dialog.run();
}

void ClockWindow::retrieve_parameters_from_gp() {

	// Masquage de la barre d'état
	if(gp->display_status_bar()) {
		statusbar.show();
	}
	else {
		statusbar.hide();
	}

	// Clavier
	curr_kbm          = &gp->keyboard_map(gp->curr_keyboard());
	curr_kam          = &gp->kam_perso();
	use_mouse_buttons =  gp->use_mouse_buttons();

	// Options de réinitialisation
	reinit_delayer.set_delay(gp->reinit_delay());
	KeyCombination reinit_kc = gp->reinit_keys();
	if(reinit_kc==DOUBLE_CTRL) {
		reinit_trigger[0] = GDK_Control_L;
		reinit_trigger[1] = GDK_Control_R;
	}
	else if(reinit_kc==DOUBLE_MAJ) {
		reinit_trigger[0] = GDK_Shift_L;
		reinit_trigger[1] = GDK_Shift_R;
	}
	else {
		assert(false);
	}

	// Options de pause
	KeyModifier pause_kc = gp->pause_keys();
	if(pause_kc==MODIFIER_VOID) {
		pause_trigger[0] = GDK_space;
		pause_trigger[1] = GDK_space;
	}
	else if(pause_kc==MODIFIER_CTRL) {
		pause_trigger[0] = GDK_Control_L;
		pause_trigger[1] = GDK_Control_R;
	}
	else if(pause_kc==MODIFIER_MAJ) {
		pause_trigger[0] = GDK_Shift_L;
		pause_trigger[1] = GDK_Shift_R;
	}
	else {
		assert(false);
	}
	pause_trigger[2] = GDK_space;
	can_pause_by_kb = gp->can_pause_by_kb();

	// Mode de réinitialisation par clic dans la barre d'outils
	reinit_confirm_mode = gp->reinit_confirm();

	// Options d'affichage
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		dial[*k].set_display_time_after_flag_down(gp->display_time_after_flag_down());
		dial[*k].set_display_bronstein_extra_time(gp->display_bronstein_extra_time());
		dial[*k].set_display_byo_yomi_extra_time (gp->display_byo_yomi_extra_time ());
	}
}

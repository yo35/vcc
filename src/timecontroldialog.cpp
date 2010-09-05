
#include "timecontroldialog.h"
#include <gtkmm/stock.h>

TimeControlDialog::TimeControlDialog(Gtk::Window &parent, const TimeControl &src) :
	Gtk::Dialog("Time control", parent, true, true)
{
	// Boutons de réponse
	add_button(Gtk::Stock::OK    , Gtk::RESPONSE_OK    );
	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	set_default_response(Gtk::RESPONSE_OK);

	// Frame "mode"
	frm_mode.set_label("Mode");
	layout_mode.set_spacing(5);
	mode[0].set_label("Simple delay");
	mode[1].set_label("Fischer"     );
	mode[2].set_label("Bronstein"   );
	mode[3].set_label("Hour-glass"  );
	for(int i=0; i<4; ++i) {
		mode[i].set_group(group);
		mode[i].signal_toggled().connect(sigc::mem_fun(*this, &TimeControlDialog::manage_sensitivity));
		layout_mode.pack_start(mode[i]);
	}
	frm_mode.add(layout_mode);

	// Frames "time"
	frm_time[0].set_label("Left" );
	frm_time[1].set_label("Right");
	for(int i=0; i<2; ++i) {
		lbl_main_time[i].set_label("Main time");
		lbl_increment[i].set_label("Increment");
		layout_time[i].resize(2, 2);
		layout_time[i].set_spacings(5);
		layout_time[i].attach(lbl_main_time[i], 0, 1, 0, 1);
		layout_time[i].attach(lbl_increment[i], 0, 1, 1, 2);
		layout_time[i].attach(    main_time[i], 1, 2, 0, 1);
		layout_time[i].attach(    increment[i], 1, 2, 1, 2);
		frm_time[i].add(layout_time[i]);
	}
	main_time[0].signal_changed().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_main_time));
	increment[0].signal_changed().connect(sigc::mem_fun(*this, &TimeControlDialog::copy_left_increment));
	layout_times.set_spacing(5);
	layout_times.pack_start(frm_time[0]);
	layout_times.pack_start(frm_time[1]);

	// Case à cocher liant les deux côtés
	link_both_times.set_label("Time control values are the same for both sides");
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
	if(mode[0].get_active()) retval.set_mode(TimeControl::SIMPLE_DELAY);
	if(mode[1].get_active()) retval.set_mode(TimeControl::FISCHER     );
	if(mode[2].get_active()) retval.set_mode(TimeControl::BRONSTEIN   );
	if(mode[3].get_active()) retval.set_mode(TimeControl::HOUR_GLASS  );
	for(int i=0; i<2; ++i) {
		retval.set_main_time(main_time[i].get_time());
		if(retval.mode()==TimeControl::FISCHER || retval.mode()==TimeControl::BRONSTEIN)
			retval.set_increment(increment[i].get_time());
	}
	return retval;
}

void TimeControlDialog::set_time_control(const TimeControl &src) {
	mode[src.mode()].set_active(true);
	for(int i=0; i<2; ++i) {
		main_time[i].set_time(src.main_time(i));
		if(src.mode()==TimeControl::FISCHER || src.mode()==TimeControl::BRONSTEIN) {
			increment[i].set_time(src.increment(i));
		}
		else {
			increment[i].set_time(0);
		}
	}
	link_both_times.set_active(src.both_sides_have_same_time());
	manage_sensitivity();
}

void TimeControlDialog::manage_sensitivity() {
	bool enable_increment = (mode[1].get_active() || mode[2].get_active());
	bool enable_right     = !link_both_times.get_active();
	increment[0].set_sensitive(enable_increment);
	increment[1].set_sensitive(enable_increment && enable_right);
	main_time[1].set_sensitive(                    enable_right);
}

void TimeControlDialog::copy_left_main_time() {
	if(link_both_times.get_active())
		main_time[1].set_time(main_time[0].get_time());
}

void TimeControlDialog::copy_left_increment() {
	if(link_both_times.get_active())
		increment[1].set_time(increment[0].get_time());
}

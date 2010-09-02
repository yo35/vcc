
#include "timeentry.h"

TimeEntry::TimeEntry() : Gtk::HBox(),
	lbl_hour("hr"), lbl_min("min"), lbl_sec("sec")
{
	set_spacing(5);
	hour.set_range(0, 99);
	min .set_range(0, 60);
	sec .set_range(0, 60);
	hour.set_increments(1, 10);
	min .set_increments(1, 10);
	sec .set_increments(1, 10);
	hour.set_value(0);
	min .set_value(0);
	sec .set_value(0);
	hour.signal_value_changed().connect(sigc::mem_fun(m_signal_changed, &sigc::signal<void>::emit));
	min .signal_value_changed().connect(sigc::mem_fun(m_signal_changed, &sigc::signal<void>::emit));
	sec .signal_value_changed().connect(sigc::mem_fun(m_signal_changed, &sigc::signal<void>::emit));
	pack_start(hour); pack_start(lbl_hour);
	pack_start(min ); pack_start(lbl_min );
	pack_start(sec ); pack_start(lbl_sec );
}

sigc::signal<void> TimeEntry::signal_changed() const {
	return m_signal_changed;
}

void TimeEntry::set_sensitive(bool sensitive) {
	hour.set_sensitive(sensitive);
	min .set_sensitive(sensitive);
	sec .set_sensitive(sensitive);
}

int TimeEntry::get_time() const {
	return
		hour.get_value_as_int()*1000*60*60 +
		min .get_value_as_int()*1000*60    +
		sec .get_value_as_int()*1000       ;
}

void TimeEntry::set_time(int new_time) {
	if(new_time<0)
		new_time = 0;
	new_time = new_time/1000; sec .set_value(new_time % 60);
	new_time = new_time/60  ; min .set_value(new_time % 60);
	new_time = new_time/60  ; hour.set_value(new_time     );
}

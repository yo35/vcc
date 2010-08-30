
#ifndef TIMEENTRY_H
#define TIMEENTRY_H

#include <gtkmm/spinbutton.h>
#include <gtkmm/box.h>

class TimeEntry : public Gtk::HBox {

public:

	// Construction
	TimeEntry();
	sigc::signal<void> signal_changed() const;

	// Modification
	void set_sensitive(bool sensitive);
	void set_time(int new_time);
	int  get_time() const;

private:

	// Signaux
	sigc::signal<void> m_signal_changed;

	// Widgets
	Gtk::SpinButton hour;
	Gtk::SpinButton min ;
	Gtk::SpinButton sec ;
	Gtk::Label      lbl_hour;
	Gtk::Label      lbl_min ;
	Gtk::Label      lbl_sec ;
};

#endif

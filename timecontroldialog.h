
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

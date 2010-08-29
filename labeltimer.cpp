
#include "labeltimer.h"

LabelTimer::LabelTimer() : AbstractTimer(), Gtk::Label() {
	Pango::AttrInt big_car = Pango::Attribute::create_attr_size(300);
	Pango::AttrInt bold    = Pango::Attribute::create_attr_weight(Pango::WEIGHT_HEAVY);
	Pango::AttrList attributes;
	attributes.insert(big_car);
	attributes.insert(bold   );
	set_attributes(attributes);
	set_size_request(300, 300);
	refresh_widget();
}

void LabelTimer::refresh_widget() {
	int curr_time = get_raw_time() / 1000;
	set_text(Glib::ustring::format(curr_time));
}


#include "digitaltimer.h"

DigitalTimer::DigitalTimer() : AbstractTimer(), Gtk::DrawingArea() {
	set_size_request(400, 400);
}

void DigitalTimer::refresh_widget() {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window == 0)
		return;

	Gtk::Allocation allocation = get_allocation();
	Gdk::Rectangle r(0, 0, allocation.get_width(), allocation.get_height());
	window->invalidate_rect(r, false);
}

bool DigitalTimer::on_expose_event(GdkEventExpose *event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window == 0)
		return false;
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
	cr->clip();

	Gtk::Allocation allocation = get_allocation();
	int width = allocation.get_width();
	int height = allocation.get_height();
		
	// Le fond de l'image
	if(get_mode()==AbstractTimer::PAUSED)
		cr->set_source_rgb(1.0, 1.0, 1.0);
	else
		cr->set_source_rgb(1.0, 1.0, 0.5);
	cr->paint();
	
	// Le text
	int curr_time = get_raw_time() / 1000;
	cr->translate(10, 300);
	cr->set_font_size(200);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->text_path(Glib::ustring::format(curr_time));
	cr->fill_preserve();
	
	return true;
}

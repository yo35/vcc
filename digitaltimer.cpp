
#include "digitaltimer.h"

DigitalTimer::DigitalTimer() : Gtk::DrawingArea() {
	m_timer = 0;
	set_size_request(400, 400);
}

void DigitalTimer::set_timer(const Timer &timer) {
	m_timer = &timer;
	m_timer->signal_modified().connect(sigc::mem_fun(*this, &DigitalTimer::refresh_widget));
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
	if(m_timer!=0 && m_timer->get_mode()==Timer::PAUSED)
		cr->set_source_rgb(1.0, 1.0, 1.0);
	else
		cr->set_source_rgb(1.0, 1.0, 0.5);
	cr->paint();

	// Le text
	if(m_timer != 0) {
		int curr_time = m_timer->get_time();
		cr->translate(10, 300);
		cr->set_font_size(200);
		cr->set_source_rgb(0.0, 0.0, 0.0);
		cr->text_path(Glib::ustring::format(curr_time));
		cr->fill_preserve();
	}

	return true;
}

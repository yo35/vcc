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


#include "dialwidget.h"
#include <libintl.h>

DialWidget::DialWidget() : Gtk::DrawingArea() {
	m_timer = 0;
	set_size_request(400, 300);
}

void DialWidget::set_timer(const Timer &timer) {
	m_timer = &timer;
	m_timer->signal_modified().connect(sigc::mem_fun(*this, &DialWidget::refresh_widget));
}

void DialWidget::refresh_widget() {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window == 0)
		return;

	Gtk::Allocation allocation = get_allocation();
	Gdk::Rectangle r(0, 0, allocation.get_width(), allocation.get_height());
	window->invalidate_rect(r, false);
}

bool DialWidget::on_expose_event(GdkEventExpose *event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window == 0)
		return false;
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
	cr->clip();
	if(m_timer==0)
		return true;

	Gtk::Allocation allocation = get_allocation();
	double width  = allocation.get_width();
	double height = allocation.get_height();

	// Le fond de l'image
	if(m_timer->get_mode()==Timer::DECREMENT)
		cr->set_source_rgb(1.0, 1.0, 0.5);
	else
		cr->set_source_rgb(1.0, 1.0, 1.0);
	cr->paint();

	// Le texte à afficher
	int curr_time = m_timer->get_time();
	int rounded_curr_time = (curr_time+499)/1000;
	Glib::ustring txt;
	if(curr_time < 0)
		txt = gettext("Flag down");
	else if(rounded_curr_time < 60*60) {
		int sec = rounded_curr_time % 60;
		int min = rounded_curr_time / 60;
		Glib::ustring txt_min = Glib::ustring::format(min);
		Glib::ustring txt_sec = Glib::ustring::format(sec);
		if(sec < 10)
			txt_sec = "0" + txt_sec;
		txt = txt_min + "." + txt_sec;
	}
	else {
		rounded_curr_time = rounded_curr_time / 60;
		int min  = rounded_curr_time % 60;
		int hour = rounded_curr_time / 60;
		Glib::ustring txt_min  = Glib::ustring::format(min );
		Glib::ustring txt_hour = Glib::ustring::format(hour);
		if(min < 10)
			txt_min = "0" + txt_min;
		txt = txt_hour + ":" + txt_min;
	}

	// Texte modèle
	std::string txt_model = txt.raw();
	for(unsigned int k=0; k<txt.size(); ++k) {
		if(txt_model[k]>='1' && txt_model[k]<='9')
			txt_model[k] = '0';
	}

	// Calcul de la position du texte
	Cairo::TextExtents te;
	double w_model = width  * 0.7;
	double h_model = height * 0.8;
	cr->set_font_size(height);
	cr->get_text_extents(txt_model, te);
	double w_scale_factor = w_model/te.width;
	double h_scale_factor = h_model/te.height;
	double scale_factor = h_scale_factor<w_scale_factor ? h_scale_factor : w_scale_factor;
	cr->set_font_size(height * scale_factor);
	cr->get_text_extents(txt_model, te);
	double delta_x = (width  - te.width )/2.0;
	double delta_y = (height + te.height)/2.0;

	// Dessin du texte
	cr->begin_new_path();
	cr->move_to(delta_x, delta_y);
	if(curr_time < 0)
		cr->set_source_rgb(0.8, 0.0, 0.0);
	else
		cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->text_path(txt);
	cr->fill_preserve();

	return true;
}

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


#include "keyboardmapwidget.h"
#include <cassert>

KeyboardMapWidget::KeyboardMapWidget() : Gtk::DrawingArea() {
	m_kbm = 0;
	set_size_request(600, 400);
}

void KeyboardMapWidget::set_keyboard_map(const KeyboardMap &kbm) {
	m_kbm = &kbm;
	refresh_widget();
}

void KeyboardMapWidget::refresh_widget() {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window == 0)
		return;

	Gtk::Allocation allocation = get_allocation();
	Gdk::Rectangle r(0, 0, allocation.get_width(), allocation.get_height());
	window->invalidate_rect(r, false);
}

bool KeyboardMapWidget::on_expose_event(GdkEventExpose *event) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window == 0)
		return false;
	cr = window->create_cairo_context();
	cr->rectangle(event->area.x, event->area.y, event->area.width, event->area.height);
	cr->clip();

	Gtk::Allocation allocation = get_allocation();
	double width  = allocation.get_width();
	double height = allocation.get_height();

	// Le fond de l'image
	cr->set_source_rgb(0.8, 1.0, 0.8);
	cr->paint();
	if(m_kbm==0)
		return true;

	// Transformation
	delta_x =  0.1*width ;
	delta_y =  0.9*height;
	scale_x =  0.8*width  / static_cast<double>(m_kbm->line_width());
	scale_y = -0.8*height / static_cast<double>(m_kbm->nb_lines());
	margin  = 0.005*(height < width ? height : width);
	radius  = 0.02 *(height < width ? height : width);

	// Touches
	cr->set_source_rgb(0.0, 0.0, 0.0);
	for(unsigned idx=0; idx<m_kbm->keys().size(); ++idx) {
		draw_key(idx);
	}

	// Rectangle 1
	/*scaled_rectangle(0, 0, m_kbm->line_width(), m_kbm->nb_lines());
	cr->set_source_rgb(1,0,0);
	cr->stroke_preserve();
	scaled_rectangle(0, 0, 1000, 4);
	cr->set_source_rgb(0,0,0);
	cr->fill_preserve();*/


	return true;
}

// Dessine une touche
void KeyboardMapWidget::draw_key(unsigned int idx) {
	assert(m_kbm!=0);
	assert(idx<m_kbm->keys().size());

	// Cas d'une touche simple
	if(m_kbm->keys()[idx].x_lines.size()==1) {
		int x0 = m_kbm->keys()[idx].x_lines.front().pos;
		int y0 = m_kbm->keys()[idx].bottom_line;
		int dx = m_kbm->keys()[idx].x_lines.front().width;
		int dy = 1;
		make_rectangle(x0, y0, dx, dy);
	}

	// Cas d'une touche située à cheval sur plusieurs lignes
	else {
		double curr_x = m_kbm->keys()[idx].x_lines.front().pos + m_kbm->keys()[idx].x_lines.front().width;
		double curr_y = m_kbm->keys()[idx].bottom_line;
		cr->move_to(curr_x*scale_x+delta_x, curr_y*scale_y+delta_y);
		for(KeySizeList::const_iterator it=m_kbm->keys()[idx].x_lines.begin();
			it!=m_kbm->keys()[idx].x_lines.end(); ++it)
		{
			curr_x = it->pos;
			cr->line_to(curr_x*scale_x+delta_x, curr_y*scale_y+delta_y);
			curr_y += 1.0;
			cr->line_to(curr_x*scale_x+delta_x, curr_y*scale_y+delta_y);
		}
		for(KeySizeList::const_reverse_iterator it=m_kbm->keys()[idx].x_lines.rbegin();
			it!=m_kbm->keys()[idx].x_lines.rend(); ++it)
		{
			curr_x = it->pos + it->width;
			cr->line_to(curr_x*scale_x+delta_x, curr_y*scale_y+delta_y);
			curr_y -= 1.0;
			cr->line_to(curr_x*scale_x+delta_x, curr_y*scale_y+delta_y);
		}
		cr->close_path();
	}

	cr->set_source_rgb(1.0, 1.0, 1.0);
	cr->fill_preserve();
}

// Dessine un rectangle échencré
void KeyboardMapWidget::make_rectangle(int x0, int y0, int dx, int dy) {
	double xl = x_conv(x0)    + margin;
	double xr = x_conv(x0+dx) - margin;
	double yb = y_conv(y0)    - margin;
	double yt = y_conv(y0+dy) + margin;
	cr->begin_new_path();
	cr->move_to(xl+radius, yb);
	cr->arc(xl+radius, yb-radius, radius, M_PI_2, M_PI);
	cr->line_to(xl, yt+radius);
	cr->arc(xl+radius, yt+radius, radius, -M_PI, -M_PI_2);
	cr->line_to(xr-radius, yt);
	cr->arc(xr-radius, yt+radius, radius, -M_PI_2, 0.0);
	cr->line_to(xr, yb-radius);
	cr->arc(xr-radius, yb-radius, radius, 0.0, M_PI_2);
	cr->close_path();

	/*cr->rectangle(
		static_cast<double>(x0)*scale_x + delta_x + margin,
		static_cast<double>(y0)*scale_y + delta_y - margin,
		static_cast<double>(dx)*scale_x - 2.0*margin,
		static_cast<double>(dy)*scale_y + 2.0*margin
	);*/
}

// Conversions x et y
double KeyboardMapWidget::x_conv(int x) const { return static_cast<double>(x)*scale_x + delta_x; }
double KeyboardMapWidget::y_conv(int y) const { return static_cast<double>(y)*scale_y + delta_y; }

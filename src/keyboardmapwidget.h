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


#ifndef KEYBOARDMAPWIDGET_H
#define KEYBOARDMAPWIDGET_H

#include <gtkmm/drawingarea.h>
#include "keyboardmap.h"

class KeyboardMapWidget : public Gtk::DrawingArea {

public:

	// Divers
	KeyboardMapWidget();
	void set_keyboard_map(const KeyboardMap &kbm);

	// Couleur et régions
	int        nb_areas()        const;
	Gdk::Color color   (int idx) const;
	void set_nb_areas(int src);
	void set_color   (int idx, const Gdk::Color &src);

protected:

	// Routine de dessin
	virtual bool on_expose_event(GdkEventExpose *event);

	// Événements clavier
	virtual bool on_key_press_event  (GdkEventKey* event);
	virtual bool on_key_release_event(GdkEventKey* event);

private:

	// Action sur une touche clavier
	void on_key_action(Keyval keyval, bool is_press);

	// Force le redessin du widget
	void refresh_widget();

	// Routines de dessin
	void draw_key_shape(unsigned int idx);
	void draw_key_text (unsigned int idx);
	void make_text     (int x0, int y0, int dx, int dy, const Glib::ustring &txt);
	void make_rectangle(int x0, int y0, int dx, int dy);
	void make_polygone(const std::list<int> &xs, const std::list<int> &ys);
	void small_arc(double xc, double yc, double r0, double angle1, double angle2);
	double x_conv(int x) const;
	double y_conv(int y) const;
	static double sgn(double src);
	static double abs(double src);
	static double min(double s1, double s2);

	// Données membres
	const KeyboardMap      *m_kbm;
	std::vector<bool>       m_keydown;
	std::vector<int >       m_keyarea;
	std::vector<Gdk::Color> m_color;

	// Objets liés au dessin du widget
	Cairo::RefPtr<Cairo::Context> cr;
	double delta_x;
  double delta_y;
  double scale_x;
	double scale_y;
	double margin ;
	double radius ;
	double padding;
	double best_sz;
};

#endif

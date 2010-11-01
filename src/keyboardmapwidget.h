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

protected:

	// Routine de dessin
	virtual bool on_expose_event(GdkEventExpose *event);

private:

	// Force le redessin du widget
	void refresh_widget();

	// Routines de dessin
	void draw_key(unsigned int idx);
	void make_rectangle(int x0, int y0, int dx, int dy);
	double x_conv(int x) const;
	double y_conv(int y) const;

	// Données membres
	const KeyboardMap *m_kbm;
	Cairo::RefPtr<Cairo::Context> cr;
	double delta_x;
  double delta_y;
  double scale_x;
	double scale_y;
	double margin ;
	double radius ;
};

#endif

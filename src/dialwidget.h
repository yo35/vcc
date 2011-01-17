/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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


#ifndef DIALWIDGET_H
#define DIALWIDGET_H

#include <gtkmm/drawingarea.h>
#include "timer.h"

class DialWidget : public Gtk::DrawingArea {

public:

	// Divers
	DialWidget();
	void set_timer(const Timer &timer);

protected:

	// Routine de dessin
	virtual bool on_expose_event(GdkEventExpose *event);

private:

	// Force le redessin du widget
	void refresh_widget();

	// Données membres
	const Timer *m_timer;
};

#endif

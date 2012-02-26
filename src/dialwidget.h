/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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
#include "side.h"
#include "bitimer.h"

class DialWidget : public Gtk::DrawingArea {

public:

	// Divers
	DialWidget();
	void set_timer(const BiTimer &bi_timer, const Side &side);

	// Options
	void set_display_time_after_flag_down(bool src);
	void set_display_bronstein_extra_time(bool src);
	void set_display_byo_yomi_extra_time (bool src);

private:

	// Routines diverses
	bool         on_timeout_elapses();
	virtual bool on_expose_event(GdkEventExpose *event);
	void         refresh_widget();
	void         draw_one_line_text (Cairo::RefPtr<Cairo::Context> cr, const std::string &txt);
	void         draw_two_lines_text(Cairo::RefPtr<Cairo::Context> cr,
		const std::string &main_txt, const std::string &extra_txt);
	double get_adjusted_font_height(Cairo::RefPtr<Cairo::Context> cr,
		int allocable_height, int allocable_width, const std::string &txt);
	static std::string get_model_txt(const std::string &src);

	// Données membres
	const BiTimer *m_bi_timer;
	Side           m_side    ;
	bool           m_display_time_after_flag_down;
	bool           m_display_bronstein_extra_time;
	bool           m_display_byo_yomi_extra_time ;
};

#endif

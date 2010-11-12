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


#ifndef AREASELECTORWIDGET_H
#define AREASELECTORWIDGET_H

#include <gtkmm/box.h>
#include <gtkmm/togglebutton.h>
#include "side.h"

class AreaSelectorWidget : public Gtk::HBox {

public:

	// Constructeur
	AreaSelectorWidget();

	// Accesseurs
	bool is_selecting() const;
	Side active_side () const;
	const Gdk::Color &color(const Side &side) const;

	// Signal
	sigc::signal<void> signal_changed() const;

private:

	// Callback déclenché lors d'une pression sur un bouton
	void on_button_clicked(Side side);

	// Modifie la couleur d'un bouton
	void   color_button(Side side);
	void uncolor_button(Side side);

	// Données membres
	EnumArray<Side, Gtk::ToggleButton> m_selector      ;
	EnumArray<Side, Gdk::Color >       m_color         ;
	bool                               m_is_selecting  ;
	Side                               m_active_side   ;
	sigc::signal<void>                 m_signal_changed;
};

#endif

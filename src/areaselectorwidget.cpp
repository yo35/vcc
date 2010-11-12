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


#include "areaselectorwidget.h"
#include <translation.h>
#include <cassert>

// Constructeur
AreaSelectorWidget::AreaSelectorWidget() : Gtk::HBox() {

	// Initialisation des données membres
	m_is_selecting = false;

	// Widgets
	m_selector[LEFT ].set_label(_("Left" ));
	m_selector[RIGHT].set_label(_("Right"));

	// Couleur
	m_color[LEFT ].set_rgb_p(0.0, 0.7, 0.0);
	m_color[RIGHT].set_rgb_p(0.0, 0.5, 1.0);

	// Opérations génériques left/right
	for(Side::iterator k=Side::first(); k.valid(); ++k) {
		color_button(*k);
		m_selector[*k].signal_released().connect(
			sigc::bind(sigc::mem_fun(*this, &AreaSelectorWidget::on_button_clicked), *k));
	}

	// Géométrie
	set_spacing(5);
	set_homogeneous(true);
	pack_start(m_selector[LEFT ]);
  pack_start(m_selector[RIGHT]);
}

// Accesseurs
bool AreaSelectorWidget::is_selecting() const {                         return m_is_selecting; }
Side AreaSelectorWidget::active_side () const { assert(m_is_selecting); return m_active_side ; }
const Gdk::Color &AreaSelectorWidget::color(const Side &side) const { return m_color[side]; }

// Signal
sigc::signal<void> AreaSelectorWidget::signal_changed() const {
	return m_signal_changed;
}

// Callback
void AreaSelectorWidget::on_button_clicked(Side side) {
	if(m_is_selecting) {

		// Si on a appuyé sur le bouton du côté en cours de sélection
		if(side==m_active_side) {
			m_is_selecting = false;
			color_button(rev(side));
		}

		// Sinon, on change de côté de sélection
		else {
			m_active_side = side;
			color_button(side);
			uncolor_button(rev(side));
			m_selector[rev(side)].set_active(false);
		}
	}

	// Sinon, on commence une séléction
	else {
		m_is_selecting = true;
		m_active_side  = side;
		uncolor_button(rev(side));
	}

	m_signal_changed.emit();
}

// Colore un bouton
void AreaSelectorWidget::color_button(Side side) {
	m_selector[side].modify_bg(Gtk::STATE_NORMAL  , m_color[side]);
	m_selector[side].modify_bg(Gtk::STATE_ACTIVE  , m_color[side]);
  m_selector[side].modify_bg(Gtk::STATE_PRELIGHT, m_color[side]);
}

// Décolore un bouton (il redevient gris)
void AreaSelectorWidget::uncolor_button(Side side) {
	m_selector[side].unset_bg(Gtk::STATE_NORMAL  );
	m_selector[side].unset_bg(Gtk::STATE_ACTIVE  );
	m_selector[side].unset_bg(Gtk::STATE_PRELIGHT);
}

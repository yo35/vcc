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


#include "keyboardmapwidget.h"
#include <cassert>
#include <cmath>


////////////////////////////////////////////////////////////////////////////////
// Fonctions de configuration générales

// Constructeur
KeyboardMapWidget::KeyboardMapWidget() : Gtk::DrawingArea() {
	m_kbm = 0;
	m_color[LEFT ].set_rgb_p(0.0, 0.7, 0.0);
	m_color[RIGHT].set_rgb_p(0.0, 0.5, 1.0);
	m_display_kp = true;
	m_is_active  = false;
	set_size_request(800, 300);
	set_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

// Structure de gestion de clavier sous-jacente
void KeyboardMapWidget::set_keyboard_map(const KeyboardMap &kbm) {
	m_kbm = &kbm;
	m_keydown.resize(m_kbm->keys().size(), false);
	m_keyarea.set_nb_keys(m_kbm->keys().size());
	refresh_widget();
}

// Couleur de la région 'idx'
const Gdk::Color &KeyboardMapWidget::color(const Side &side) const {
	return m_color[side];
}

// Affichage du pavé numérique
bool KeyboardMapWidget::display_kp() const {
	return m_display_kp;
}

// Modification de l'affichage du pavé num.
void KeyboardMapWidget::set_display_kp(bool src) {
	m_display_kp = src;
	refresh_widget();
}



////////////////////////////////////////////////////////////////////////////////
// Exploitation des régions de touches

// Modifie la région active
void KeyboardMapWidget::set_active_side(const Side &side) { m_is_active=true ; m_active_side=side; }
void KeyboardMapWidget::unset_active_side()               { m_is_active=false;                     }

// Région en train d'être définie
bool KeyboardMapWidget::is_active  () const { return m_is_active  ; }
Side KeyboardMapWidget::active_side() const { return m_active_side; }

// Récupération des régions de touches
const AreaMap &KeyboardMapWidget::get_area_map() const {
	return m_keyarea;
}

// Définition des régions de touches
void KeyboardMapWidget::set_area_map(const AreaMap &src) {
	assert(m_kbm!=0 && static_cast<int>(m_kbm->keys().size())==src.nb_keys());
	m_keyarea = src;
	refresh_widget();
}

// Vide toutes les régions
void KeyboardMapWidget::clear_areas() {
	m_keyarea.clear();
	refresh_widget();
}



////////////////////////////////////////////////////////////////////////////////
// Gestion des événements

bool KeyboardMapWidget::on_button_press_event(GdkEventButton *event) {

	// Vérifications préliminaires
	if(m_kbm==0 || event->button!=1 || !m_is_active)
		return true;

	// Recherche de la touche désignée par le curseur
	int key = lookup_key_at(event->x, event->y);
	if(key<0)
		return true;
	assert(key<=m_keyarea.nb_keys());

	// Modif
	if(m_keyarea.is_affected(key) && m_keyarea.side(key)==m_active_side)
		m_keyarea.set_unaffected(key);
	else
		m_keyarea.set_side(key, m_active_side);
	refresh_widget();
	return true;
}

void KeyboardMapWidget::on_key_action(Keyval keyval, bool is_press) {
	if(m_kbm==0)
		return;

	int key = m_kbm->get_key(keyval);
	if(key>=0) {
		assert(key<static_cast<int>(m_keydown.size()));
		if(m_keydown[key]==is_press)
			return;
		m_keydown[key] = is_press;
		refresh_widget();
	}
}



////////////////////////////////////////////////////////////////////////////////
// Routines de dessin

int KeyboardMapWidget::lookup_key_at(double x, double y) const {
	if(m_kbm==0)
		return -1;

	int no_line = y_deconv(y);
	int xcurs   = x_deconv(x);
	for(unsigned idx=0; idx<m_kbm->keys().size(); ++idx) {

		// On check la ligne
		if(no_line >= m_kbm->keys()[idx].bottom_line() &&
			no_line < m_kbm->keys()[idx].bottom_line()+m_kbm->keys()[idx].nb_lines())
		{
			int p = no_line - m_kbm->keys()[idx].bottom_line();

			// On check la colonne
			if(xcurs >= m_kbm->keys()[idx].pos_on_line(p) &&
				xcurs < m_kbm->keys()[idx].pos_on_line(p)+m_kbm->keys()[idx].width_on_line(p))
			{
				return idx;
			}
		}
	}
	return -1;
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
	double width  = allocation.get_width ();
	double height = allocation.get_height();

	// Le fond de l'image
	cr->set_source_rgb(0.8, 1.0, 0.8);
	cr->paint();
	if(m_kbm==0)
		return true;

	// Transformation
	double border_x = 15;
	double border_y = 10;
	width   = width  - 2.0*border_x;
	height  = height - 2.0*border_y;
	double obj_width  = static_cast<double>(m_kbm->line_width(m_display_kp));
	double obj_height = static_cast<double>(m_kbm->default_width() * m_kbm->nb_lines());
	double pseudo_scale_x = width  / obj_width ;
	double pseudo_scale_y = height / obj_height;
	double pseudo_scale   = min(pseudo_scale_x, pseudo_scale_y);
	scale_x =  pseudo_scale;
	scale_y = -pseudo_scale * static_cast<double>(m_kbm->default_width());
	delta_x = (width  - obj_width *pseudo_scale) / 2.0 + border_x;
	delta_y = (height + obj_height*pseudo_scale) / 2.0 + border_y;
	margin  = 0.007*min(height, width);
	radius  = 0.02 *min(height, width);
	big_pad = 0.03 *min(height, width);
	sml_pad = 0.02 *min(height, width);

	// Touches
	for(unsigned idx=0; idx<m_kbm->keys().size(); ++idx) {
		if(!m_display_kp && m_kbm->keys()[idx].in_kp())
			continue;
		draw_key_shape(idx);
		draw_key_text (idx);
	}
	return true;
}

// Écrit le texte de la touche
void KeyboardMapWidget::draw_key_text(unsigned int idx) {
	assert(m_kbm!=0);
	assert(idx<m_kbm->keys().size());

	// Ligne sur laquelle sera écrit le texte (la plus large possible)
	int no_larger_line = 0;
	int pos_on_line    = 0;
	int larger_width   = 0;
	for(int i=0; i<m_kbm->keys()[idx].nb_lines(); ++i) {
		if(m_kbm->keys()[idx].width_on_line(i) >= larger_width) {
			no_larger_line = m_kbm->keys()[idx].bottom_line() + i;
			pos_on_line    = m_kbm->keys()[idx].pos_on_line  (i);
			larger_width   = m_kbm->keys()[idx].width_on_line(i);
		}
	}

	// Coordonnées de la touche
	double xl = x_conv(pos_on_line             );
	double xr = x_conv(pos_on_line+larger_width);
	double yb = y_conv(no_larger_line          );
	double yt = y_conv(no_larger_line+1        );
	double xm = (xl+xr) / 2.0;
	double ym = (yt+yb) / 2.0;

	// Texte à écrire
	Glib::ustring txts[4];
	int nb_txts = 0;
	for(int k=0; k<m_kbm->keys()[idx].nb_keyvals(); ++k) {
		if(m_kbm->keys()[idx].print(k)) {
			txts[nb_txts] = keyval_to_symbol(m_kbm->keys()[idx].keyval(k));
			++nb_txts;
			if(nb_txts==4)
				break;
		}
	}

	// Coupures de ligne
	bool same_text = false;
	if(nb_txts==1) {
		size_t pos_coupure = txts[0].find("\n");
		if(pos_coupure!=Glib::ustring::npos) {
			txts[1]   = txts[0].substr(0, pos_coupure);
			txts[0]   = txts[0].substr(pos_coupure+1);
			nb_txts   = 2;
			same_text = true;
		}
	}

	// Écrit le texte
	if(nb_txts==0)
		return;
	cr->set_source_rgb(0.0, 0.0, 0.0);
	double big_font_size   = 0.7;
	double small_font_size = 0.4;
	double actual_size;

	// Si un seul texte
	if(nb_txts==1) {
		actual_size = compute_text_size(xl+big_pad, xr-big_pad, yt, yb, txts[0], small_font_size);
		make_text                      (xl+big_pad, xr-big_pad, yt, yb, txts[0], actual_size    );
		cr->fill_preserve();
	}

	// Si deux textes
	else if(nb_txts==2) {
		if(same_text) {
			double sz_bot = compute_text_size(xl+big_pad, xr-big_pad, ym, yb-sml_pad, txts[0], big_font_size);
			double sz_top = compute_text_size(xl+big_pad, xr-big_pad, yt+sml_pad, ym, txts[1], big_font_size);
			actual_size = min(sz_bot, sz_top);
			make_text(xl+big_pad, xr-big_pad, ym, yb-sml_pad, txts[0], actual_size);
			cr->fill_preserve();
			make_text(xl+big_pad, xr-big_pad, yt+sml_pad, ym, txts[1], actual_size);
			cr->fill_preserve();
		}
		else {
			actual_size = compute_text_size(xl+big_pad, xr-big_pad, ym, yb-sml_pad, txts[0], big_font_size);
			make_text                      (xl+big_pad, xr-big_pad, ym, yb-sml_pad, txts[0], actual_size  );
			cr->fill_preserve();
			actual_size = compute_text_size(xl+big_pad, xr-big_pad, yt+sml_pad, ym, txts[1], big_font_size);
			make_text                      (xl+big_pad, xr-big_pad, yt+sml_pad, ym, txts[1], actual_size  );
			cr->fill_preserve();
		}
	}

	// Si 3 ou 4 textes
	else {
		actual_size = compute_text_size(xl+sml_pad, xm, ym, yb-sml_pad, txts[0], big_font_size);
		make_text                      (xl+sml_pad, xm, ym, yb-sml_pad, txts[0], actual_size  );
		cr->fill_preserve();
		actual_size = compute_text_size(xl+sml_pad, xm, yt+sml_pad, ym, txts[1], big_font_size);
		make_text                      (xl+sml_pad, xm, yt+sml_pad, ym, txts[1], actual_size  );
		cr->fill_preserve();
		actual_size = compute_text_size(xm, xr-sml_pad, ym, yb-sml_pad, txts[2], big_font_size);
		make_text                      (xm, xr-sml_pad, ym, yb-sml_pad, txts[2], actual_size  );
		cr->fill_preserve();
		if(nb_txts>=4) {
			actual_size = compute_text_size(xm, xr-sml_pad, yt+sml_pad, ym, txts[3], big_font_size);
			make_text                      (xm, xr-sml_pad, yt+sml_pad, ym, txts[3], actual_size  );
			cr->fill_preserve();
		}
	}
}

// Calcule la taille d'un texte devant être écrit dans le rectangle passé en
// paramètre
double KeyboardMapWidget::compute_text_size(double xl, double xr, double yt, double yb,
	const Glib::ustring &txt, double expected_relative_font_size)
{
	double w_box     = xr-xl;
	double h_box     = yb-yt;
	double font_size = expected_relative_font_size * h_box;
	cr->set_font_size(font_size);
	Cairo::TextExtents te;
	cr->get_text_extents(txt, te);
	if(te.width > w_box)
		font_size = font_size * w_box / te.width;
	return font_size;
}

// Dessine un texte centré dans le rectangle passé en paramètre
void KeyboardMapWidget::make_text(double xl, double xr, double yt, double yb,
	const Glib::ustring &txt, double font_size)
{
	// Initialisation
	cr->begin_new_path();
	cr->set_font_size(font_size);

	// Taille du texte
	double w_box = xr-xl;
	double h_box = yb-yt;
	Cairo::TextExtents te;
	cr->get_text_extents(txt, te);
	Cairo::TextExtents te_model;
	cr->get_text_extents("M", te_model);

	// Dessin
	double delta_x_box = (w_box - te      .width )/2.0;
	double delta_y_box = (h_box + te_model.height)/2.0;
	cr->begin_new_path();
	cr->move_to(xl+delta_x_box, yt+delta_y_box);
	cr->text_path(txt);
}

// Dessine la touche sans le texte
void KeyboardMapWidget::draw_key_shape(unsigned int idx) {
	assert(m_kbm!=0);
	assert(idx<m_kbm->keys().size());

	// Cas d'une touche simple
	if(m_kbm->keys()[idx].nb_lines()==1) {
		int x0 = m_kbm->keys()[idx].pos_on_line(0);
		int y0 = m_kbm->keys()[idx].bottom_line();
		int dx = m_kbm->keys()[idx].width_on_line(0);
		int dy = 1;
		make_rectangle(x0, y0, dx, dy);
	}

	// Cas d'une touche située à cheval sur plusieurs lignes
	else {
		std::list<int> xs;
		std::list<int> ys;
		for(int i=0; i<m_kbm->keys()[idx].nb_lines(); ++i) {
			xs.push_back(m_kbm->keys()[idx].pos_on_line(i));
		}
		for(int i=m_kbm->keys()[idx].nb_lines()-1; i>=0; --i) {
			xs.push_back(m_kbm->keys()[idx].pos_on_line(i) + m_kbm->keys()[idx].width_on_line(i));
		}
		int first_x = xs.back();
		xs.pop_back();
		xs.push_front(first_x);
		for(int dy=0; dy<m_kbm->keys()[idx].nb_lines(); ++dy) {
			ys.push_back(m_kbm->keys()[idx].bottom_line() + dy);
		}
		for(int dy=m_kbm->keys()[idx].nb_lines(); dy>0; --dy) {
			ys.push_back(m_kbm->keys()[idx].bottom_line() + dy);
		}
		make_polygone(xs, ys);
	}

	// Applique la couleur
	if(m_keydown[idx]) {
		cr->set_source_rgb(1.0, 0.5, 0.0);
	}
	else if(!m_keyarea.is_affected(idx)) {
		cr->set_source_rgb(1.0, 1.0, 1.0);
	}
	else {
		Gdk::Color curr_color = m_color[m_keyarea.side(idx)];
		cr->set_source_rgb(
			curr_color.get_red_p  (),
			curr_color.get_green_p(),
			curr_color.get_blue_p ()
		);
	}
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
	cr->arc(xl+radius, yb-radius, radius, M_PI_2, M_PI);   // SO
	cr->line_to(xl, yt+radius);
	cr->arc(xl+radius, yt+radius, radius, -M_PI, -M_PI_2); // NO
	cr->line_to(xr-radius, yt);
	cr->arc(xr-radius, yt+radius, radius, -M_PI_2, 0.0);   // NE
	cr->line_to(xr, yb-radius);
	cr->arc(xr-radius, yb-radius, radius, 0.0, M_PI_2);    // SE
	cr->close_path();
}

// Dessine un polygone échencré
void KeyboardMapWidget::make_polygone(const std::list<int> &xs, const std::list<int> &ys) {
	assert(xs.size()==ys.size());
	assert(xs.size()>=2);

	// Les indices pairs sont des x (arètes verticales),
	// les indices impairs des y (arètes horizontales)
	std::vector<double> aretes(2*xs.size()+2);
	std::list<int>::const_iterator x = xs.begin();
	std::list<int>::const_iterator y = ys.begin();
	for(unsigned int idx=0; idx<xs.size(); ++idx) {
		aretes[2*idx  ] = x_conv(*x);
		aretes[2*idx+1] = y_conv(*y);
		++x;
		++y;
	}
	aretes[2*xs.size()  ] = aretes[0];
	aretes[2*xs.size()+1] = aretes[1];

	// Corrections margin
	std::vector<double> aretes_margin = aretes;
	for(unsigned int idx=0; idx<xs.size(); ++idx) {
		aretes_margin[2*idx+1] += -margin * sgn(aretes[2*idx  ] - aretes[2*idx+2]);
		aretes_margin[2*idx+2] +=  margin * sgn(aretes[2*idx+1] - aretes[2*idx+3]);
	}
	aretes_margin[0] = aretes_margin[2*xs.size()];
	aretes_margin[2*xs.size()+1] = aretes_margin[1];

	// Rayon ajusté du début de l'arète
	std::vector<double> radius_aj(2*xs.size()+2);
	for(unsigned int idx=0; idx<=2*xs.size()+1; ++idx) {
		radius_aj[idx] = radius;
	}
	for(unsigned int idx=1; idx<=2*xs.size(); ++idx) {
		radius_aj[idx] = min(radius_aj[idx], abs(aretes_margin[idx-1]-aretes_margin[idx+1])/2);
	}
	radius_aj[0] = radius_aj[2*xs.size()];
	radius_aj[2*xs.size()+1] = radius_aj[1];
	for(unsigned int idx=2; idx<=2*xs.size()+1; ++idx) {
		radius_aj[idx] = min(radius_aj[idx], abs(aretes_margin[idx]-aretes_margin[idx-2])/2);
	}
	radius_aj[0] = radius_aj[2*xs.size()  ];
	radius_aj[1] = radius_aj[2*xs.size()+1];

	// Coordonnées de stop des arètes
	// Rq : inversion de la parité
	std::vector<double> start_at(2*xs.size()+2);
	std::vector<double> stop_at (2*xs.size()+2);
	for(unsigned int idx=1; idx<=2*xs.size(); ++idx) {
		start_at[idx] = aretes_margin[idx-1] - radius_aj[idx]  *sgn(aretes[idx-1]-aretes[idx+1]);
		stop_at [idx] = aretes_margin[idx+1] + radius_aj[idx+1]*sgn(aretes[idx-1]-aretes[idx+1]);
	}
	start_at[0] = start_at[2*xs.size()];
	stop_at [0] = stop_at [2*xs.size()];
	start_at[2*xs.size()+1] = start_at[1];
	stop_at [2*xs.size()+1] = stop_at [1];

	// Angles de départ au début de l'arète
	std::vector<double> angle_dep(2*xs.size()+2);
	for(unsigned int idx=0; idx<xs.size(); ++idx) {
		angle_dep[2*idx+1] = (aretes[2*idx+2] < aretes[2*idx  ]) ? 0.0 : -M_PI;
		angle_dep[2*idx+2] = (aretes[2*idx+3] < aretes[2*idx+1]) ? M_PI_2 : -M_PI_2;
	}
	angle_dep[0] = angle_dep[2*xs.size()];
	angle_dep[2*xs.size()+1] = angle_dep[1];

	// Angles d'arrivée au début de l'arète
	std::vector<double> angle_arr(angle_dep);
	for(unsigned int idx=0; idx<xs.size(); ++idx) {
		angle_arr[2*idx+2] +=  M_PI_2*sgn(aretes[2*idx+3]-aretes[2*idx+1])*sgn(aretes[2*idx+2]-aretes[2*idx  ]);
		angle_arr[2*idx+3] += -M_PI_2*sgn(aretes[2*idx+3]-aretes[2*idx+1])*sgn(aretes[2*idx+4]-aretes[2*idx+2]);
	}
	angle_arr[0] = angle_arr[2*xs.size()  ];
	angle_arr[1] = angle_arr[2*xs.size()+1];

	// Tracé
	cr->begin_new_path();
	cr->move_to(aretes_margin[0], stop_at[0]);
	for(unsigned int idx=0; idx<xs.size(); ++idx) {
		small_arc(start_at[2*idx+1], stop_at[2*idx], radius_aj[2*idx+1],
			angle_dep[2*idx+1], angle_arr[2*idx+1]);
		cr->line_to(stop_at [2*idx+1], aretes_margin[2*idx+1]);
		small_arc(stop_at[2*idx+1], start_at[2*idx+2], radius_aj[2*idx+2],
			angle_dep[2*idx+2], angle_arr[2*idx+2]);
		cr->line_to(aretes_margin[2*idx+2], stop_at [2*idx+2]);
	}
	cr->close_path();
}

// Arc de cercle
void KeyboardMapWidget::small_arc(double xc, double yc, double r0, double angle1, double angle2) {
	if(angle1 < angle2)
		cr->arc(xc, yc, r0, angle1, angle2);
	else
		cr->arc_negative(xc, yc, r0, angle1, angle2);
}

// Conversions x et y (coordonnées clavier vers widget)
double KeyboardMapWidget::x_conv(int x) const { return static_cast<double>(x)*scale_x + delta_x; }
double KeyboardMapWidget::y_conv(int y) const { return static_cast<double>(y)*scale_y + delta_y; }

// Conversions x et y (coordonnées widget vers clavier)
int KeyboardMapWidget::x_deconv(double x) const { return floor((x-delta_x) / scale_x); }
int KeyboardMapWidget::y_deconv(double y) const { return floor((y-delta_y) / scale_y); }

// Fonctions maths
double KeyboardMapWidget::sgn(double src) {
	return src==0 ? 0 : (src>0 ? 1 : -1);
}
double KeyboardMapWidget::abs(double src) {
	return src<0 ? -src : src;
}
double KeyboardMapWidget::min(double s1, double s2) {
	return s1 < s2 ? s1 : s2;
}

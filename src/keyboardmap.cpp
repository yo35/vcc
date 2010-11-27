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


#include "keyboardmap.h"
#include "datafile.h"
#include <translation.h>
#include <stdexcept>
#include <cassert>
#include <glibmm/ustring.h>

// Constructeur
KeyboardMap::KeyboardMap() {
	m_nb_lines              =    7;
	m_default_width         =  100;
	m_line_width_with_kp    = 2200;
	m_line_width_without_kp = 1800;
	translation_table_ready = false;
}

// Accesseurs
int KeyboardMap::nb_lines     ()             const { return m_nb_lines     ; }
int KeyboardMap::default_width()             const { return m_default_width; }
int KeyboardMap::line_width   (bool with_kp) const { return with_kp ? m_line_width_with_kp : m_line_width_without_kp; }
const PhysicalKeyVector &KeyboardMap::keys() const { return m_keys         ; }

// Traduction keyval -> physical key
int KeyboardMap::get_key(Keyval keyval) const {
	assert(translation_table_ready);
	std::map<Keyval, int>::const_iterator it = translation_table.find(keyval);
	if(it==translation_table.end())
		return -1;
	else
		return it->second;
}

// Écriture dans un fichier
void KeyboardMap::save(const std::string &path) const {

	// Ouverture du fichier cible
	DataFileOut file(path);
	file.open();

	// En-tête du fichier
	file.put(1                      );
	file.put(m_nb_lines             );
	file.put(m_default_width        );
	file.put(m_line_width_with_kp   );
	file.put(m_line_width_without_kp);
	file.put(m_keys.size()          );

	// Boucle d'écriture pour chaque touche
	for(PhysicalKeyVector::const_iterator it=m_keys.begin(); it!=m_keys.end(); ++it) {
		file.put(it->bottom_line());
		file.put(it->in_kp      () ? 1 : 0);
		file.put(it->nb_lines   ());
		file.put(it->nb_keyvals ());
		for(int i=0; i<it->nb_lines(); ++i) {
			file.put(it->pos_on_line  (i));
			file.put(it->width_on_line(i));
		}
		for(int i=0; i<it->nb_keyvals(); ++i) {
			file.put(it->keyval(i));
			file.put(it->group (i));
			file.put(it->level (i));
			file.put(it->print (i) ? 1 : 0);
		}
	}

	// Fermeture du fichier
	file.close();
}

// Chargement depuis un fichier
void KeyboardMap::load(const std::string &path) {
	translation_table_ready = false;

	// Ouverture du fichier source
	DataFileIn file(path);
	file.open();

	// Numéro de version
	int no_version = file.get();
	if(no_version!=1) {
		throw std::runtime_error(Glib::ustring::compose(
			_("The version (%1) of the keyboard map file %2 is not supported"), no_version, path));
	}

	// Ligne d'en-tête
	m_nb_lines              = file.get();
	m_default_width         = file.get();
	m_line_width_with_kp    = file.get();
	m_line_width_without_kp = file.get();
	int nb_keys             = file.get();
	m_keys.resize(nb_keys);

	// Boucle de lecture pour chaque touche
	for(int idx=0; idx<nb_keys; ++idx) {

		// Index de la première ligne, nombre de lignes et de keyvals de la touche
		int  bottom_line      = file.get();
		bool in_kp            = file.get()!=0;
		int  nb_line_of_key   = file.get();
		int  nb_keyval_of_key = file.get();
		m_keys[idx].set_bottom_line(bottom_line     );
		m_keys[idx].set_in_kp      (in_kp           );
		m_keys[idx].set_nb_lines   (nb_line_of_key  );
		m_keys[idx].set_nb_keyvals (nb_keyval_of_key);

		// Liste des coordonnées de la touche sur chaque ligne du clavier
		if(nb_line_of_key<1) {
			throw std::runtime_error(Glib::ustring::compose(
				_("The key %1 must be placed at least on one line on the keyboard"), idx));
		}
		for(int i=0; i<nb_line_of_key; ++i) {
			int curr_pos   = file.get();
			int curr_width = file.get();
			m_keys[idx].set_geometry(i, curr_pos, curr_width);
		}

		// Liste des keyvals
		if(nb_keyval_of_key<1) {
			throw std::runtime_error(Glib::ustring::compose(
				_("The key %1 must have at least one keyval"), idx));
		}
		for(int i=0; i<nb_keyval_of_key; ++i) {
			Keyval   curr_keyval = file.get();
			KeyGroup curr_group  = file.get();
			KeyLevel curr_level  = file.get();
			bool     curr_print  = file.get()!=0;
			m_keys[idx].set_keyval(i, curr_keyval, curr_group, curr_level, curr_print);
		}
	}

	// Terminaison
	file.close();
	compute_translation_table();
}

// Calcul de la table de traduction
void KeyboardMap::compute_translation_table() {
	translation_table.clear();
	for(int idx=0; idx<static_cast<int>(m_keys.size()); ++idx) {
		for(int k=0; k<m_keys[idx].nb_keyvals(); ++k) {
			Keyval curr_keyval = m_keys[idx].keyval(k);
			if(translation_table.find(curr_keyval)==translation_table.end())
				translation_table[curr_keyval] = idx;
			else if(translation_table[curr_keyval]!=idx)
				translation_table[curr_keyval] = -1;
		}
	}
	translation_table_ready = true;
}

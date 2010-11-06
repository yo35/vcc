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
#include "strings.h"
#include <translation.h>
#include <stdexcept>
#include <cassert>
#include <glibmm/ustring.h>

// Constructeur
KeyboardMap::KeyboardMap() {
	m_nb_lines              =    7;
	m_default_width         =  100;
	m_line_width            = 2200;
	translation_table_ready = false;
}

// Accesseurs
int                      KeyboardMap::nb_lines     () const { return m_nb_lines     ; }
int                      KeyboardMap::default_width() const { return m_default_width; }
int                      KeyboardMap::line_width   () const { return m_line_width   ; }
const PhysicalKeyVector &KeyboardMap::keys         () const { return m_keys         ; }

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
	std::ofstream file;
	file.open(path.c_str());
	if(file.fail()) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to open the keyboard map file %1 for writing"), path));
	}

	// En-tête du fichier
	file << m_nb_lines << ";" << m_default_width << ";" << m_line_width << ";"
		<< m_keys.size() << ";" << std::endl;

	// Boucle d'écriture pour chaque touche
	for(PhysicalKeyVector::const_iterator it=m_keys.begin(); it!=m_keys.end(); ++it) {
		file << it->bottom_line() << ";" << it->nb_lines() << ";" << it->nb_keyvals() << ";";
		for(int i=0; i<it->nb_lines(); ++i) {
			file << it->pos_on_line(i) << ";" << it->width_on_line(i) << ";";
		}
		for(int i=0; i<it->nb_keyvals(); ++i) {
			file << it->keyval(i) << ";" << it->group(i) << ";" << it->level(i) << ";";
		}
		file << std::endl;
	}

	// Fermeture du fichier
	file.close();
}

// Chargement depuis un fichier
void KeyboardMap::load(const std::string &path) {
	translation_table_ready = false;

	// Ouverture du fichier source
	std::ifstream file;
	file.open(path.c_str());
	if(file.fail()) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to open the keyboard map file %1 for reading"), path));
	}

	// Initialisation
	int curr_no_line = 0;
	std::list<int> curr_line;
	m_keys.clear();

	// Ligne d'en-tête
	bool first_line_ok = parse_line(file, path, curr_no_line, curr_line);
	check_consistency(first_line_ok && curr_line.size()==4, path, curr_no_line);
	m_nb_lines      = curr_line.front(); curr_line.pop_front();
	m_default_width = curr_line.front(); curr_line.pop_front();
	m_line_width    = curr_line.front(); curr_line.pop_front();
	int nb_keys     = curr_line.front(); curr_line.pop_front();
	m_keys.resize(nb_keys);

	// Boucle de lecture pour chaque touche
	unsigned int idx = 0;
	while(parse_line(file, path, curr_no_line, curr_line)) {

		// Index de la première ligne, nombre de lignes et de keyvals de la touche
		check_consistency(curr_line.size()>=3, path, curr_no_line);
		int bottom_line      = curr_line.front(); curr_line.pop_front();
		int nb_line_of_key   = curr_line.front(); curr_line.pop_front();
		int nb_keyval_of_key = curr_line.front(); curr_line.pop_front();
		m_keys[idx].set_bottom_line(bottom_line     );
		m_keys[idx].set_nb_lines   (nb_line_of_key  );
		m_keys[idx].set_nb_keyvals (nb_keyval_of_key);

		// Liste des coordonnées de la touche sur chaque ligne du clavier
		check_consistency(nb_line_of_key>=1 && static_cast<int>(curr_line.size())>=2*nb_line_of_key,
			path, curr_no_line);
		for(int i=0; i<nb_line_of_key; ++i) {
			int curr_pos   = curr_line.front(); curr_line.pop_front();
			int curr_width = curr_line.front(); curr_line.pop_front();
			m_keys[idx].set_geometry(i, curr_pos, curr_width);
		}

		// Liste des keyvals
		check_consistency(nb_keyval_of_key>=1 && static_cast<int>(curr_line.size())>=3*nb_keyval_of_key,
			path, curr_no_line);
		for(int i=0; i<nb_keyval_of_key; ++i) {
			Keyval   curr_keyval = curr_line.front(); curr_line.pop_front();
			KeyGroup curr_group  = curr_line.front(); curr_line.pop_front();
			KeyLevel curr_level  = curr_line.front(); curr_line.pop_front();
			m_keys[idx].set_keyval(i, curr_keyval, curr_group, curr_level);
		}

		// Touche suivante
		check_consistency(curr_line.empty(), path, curr_no_line);
		++idx;
	}

	// Terminaison
	check_consistency(idx==m_keys.size(), path, curr_no_line);
	file.close();
	compute_translation_table();
}

// Lecture d'une ligne
bool KeyboardMap::parse_line(std::ifstream &file, const std::string &path,
	int &curr_no_line, std::list<int> &retval)
{
	// Trouve la première ligne non-vide qui ne soit pas un commentaire
	std::string curr_str_line;
	while(!file.eof()) {
		getline(file, curr_str_line);
		++curr_no_line;
		curr_str_line = trim(curr_str_line);
		if(!(curr_str_line.empty() || curr_str_line.at(0)=='#'))
			break;
	}
	if(file.eof())
		return false;

	// Suppression du commentaire en fin de ligne éventuel
	size_t pos_diese = curr_str_line.find('#');
	if(pos_diese!=std::string::npos) {
		curr_str_line = trim(curr_str_line.substr(0, pos_diese));
	}

	// Découpe la liste en fonction du séparateur ';'
	retval.clear();
	std::list<std::string> items = split(curr_str_line, ';');
	for(std::list<std::string>::const_iterator it=items.begin(); it!=items.end(); ++it) {
		std::string curr_item = trim(*it);
		if(curr_item=="")
			continue;
		check_consistency(is_valid_int(curr_item), path, curr_no_line);
		retval.push_back(string_to_int(curr_item));
	}
	return true;
}

// Routine de vérification (lance une exception si test est faux
void KeyboardMap::check_consistency(bool test, const std::string &path, int curr_no_line) {
	if(!test) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unconsistency in the keyboard map file %1 at line #%2"), path, curr_no_line));
	}
}

// Calcul de la table de traduction
void KeyboardMap::compute_translation_table() {
	translation_table.clear();
	for(unsigned int idx=0; idx<m_keys.size(); ++idx) {
		for(int k=0; k<m_keys[idx].nb_keyvals(); ++k) {
			Keyval curr_keyval = m_keys[idx].keyval(k);
			if(translation_table.find(curr_keyval)==translation_table.end()) {
				translation_table[curr_keyval] = static_cast<int>(idx);
			} else {
				translation_table[curr_keyval] = -1;
			}
		}
	}
	translation_table_ready = true;
}

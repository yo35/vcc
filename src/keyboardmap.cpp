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
#include <glibmm/ustring.h>

// Constructeur
KeyboardMap::KeyboardMap() {
	m_nb_lines      =    6;
	m_default_width =  100;
	m_line_width    = 2200;
}

// Accesseurs
int                      KeyboardMap::nb_lines     () const { return m_nb_lines     ; }
int                      KeyboardMap::default_width() const { return m_default_width; }
int                      KeyboardMap::line_width   () const { return m_line_width   ; }
const PhysicalKeyVector &KeyboardMap::keys         () const { return m_keys         ; }

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
		file << it->x_lines.size() << ";" << it->bottom_line << ";";
		for(KeySizeList::const_iterator i=it->x_lines.begin(); i!=it->x_lines.end(); ++i) {
			file << i->pos << ";" << i->width << ";";
		}
		for(KeyvalList::const_iterator i=it->keyvals.begin(); i!=it->keyvals.end(); ++i) {
			file << *i << ";";
		}
		file << std::endl;
	}

	// Fermeture du fichier
	file.close();
}

// Chargement depuis un fichier
void KeyboardMap::load(const std::string &path) {

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
		PhysicalKey key;
		int nb_line_on_kb = 0;

		// Nombre de ligne de la touche courante
		check_consistency(!curr_line.empty(), path, curr_no_line);
		nb_line_on_kb = curr_line.front(); curr_line.pop_front();

		// Numéro de la première ligne de la touche courante
		check_consistency(!curr_line.empty(), path, curr_no_line);
		key.bottom_line = curr_line.front(); curr_line.pop_front();

		// Liste des coordonnées de la touche sur chaque ligne du clavier
		while(nb_line_on_kb>0) {
			KeySize sz;
			check_consistency(!curr_line.empty(), path, curr_no_line);
			sz.pos = curr_line.front(); curr_line.pop_front();
			check_consistency(!curr_line.empty(), path, curr_no_line);
			sz.width = curr_line.front(); curr_line.pop_front();
			key.x_lines.push_back(sz);
			--nb_line_on_kb;
		}

		// Liste des keyvals associées à la touche
		while(!curr_line.empty()) {
			Keyval curr_keyval = curr_line.front(); curr_line.pop_front();
			key.keyvals.push_back(curr_keyval);
		}
		check_consistency(!key.keyvals.empty(), path, curr_no_line);

		// Enregistrement de la touche courante
		check_consistency(idx<m_keys.size(), path, curr_no_line);
		m_keys[idx] = key;
		++idx;
	}

	// Terminaison
	check_consistency(idx==m_keys.size(), path, curr_no_line);
	file.close();
}

// Lecture d'une ligne
bool KeyboardMap::parse_line(std::ifstream &file, const std::string &path,
	int &curr_no_line, std::list<int> &retval)
{
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

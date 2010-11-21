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


#include "inistruct.h"
#include "strings.h"
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <glibmm/ustring.h>
#include <translation.h>

// Lecture
void IniStruct::load(const std::string &path) {

	// Ouverture du fichier source
	std::ifstream file;
	file.open(path.c_str());
	if(file.fail()) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to open the INI file %1 for reading"), path));
	}

	// Initialisations
	Key curr_section = "";
	int curr_no_line = 0;
	root.clear();

	// Boucle de lecture
	while(!file.eof()) {
		std::string line;
		getline(file, line);
		++curr_no_line;
		line = trim(line);

		// Cas d'une ligne vide ou d'un commentaire
		if(line.empty() || line.at(0)=='#') {
			// RAS
			continue;
		}

		// Cas d'un titre de section
		else if(line.at(0)=='[') {
			if(line.length()<2 || line.at(line.length()-1)!=']') {
				throw std::runtime_error(Glib::ustring::compose(
					_("Unconsistency in the INI file %1 at line #%2"), path, curr_no_line));
			}
			curr_section = line.substr(1, line.length()-2);
			if(!is_valid_key(curr_section)) {
				throw std::runtime_error(Glib::ustring::compose(
					_("Unconsistency in the INI file %1 at line #%2"), path, curr_no_line));
			}
		}

		// Cas d'une ligne clé=valeur
		else if(is_valid_id_char(line.at(0))) {
			if(curr_section.empty()) {
				throw std::runtime_error(Glib::ustring::compose(
					_("Unconsistency in the INI file %1 at line #%2"), path, curr_no_line));
			}
			size_t pos_eq = line.find('=');
			if(pos_eq==std::string::npos) {
				throw std::runtime_error(Glib::ustring::compose(
					_("Unconsistency in the INI file %1 at line #%2"), path, curr_no_line));
			}
			Key   key = trim(line.substr(0, pos_eq));
			Value val = trim(line.substr(pos_eq+1));
			if(!is_valid_key(key)) {
				throw std::runtime_error(Glib::ustring::compose(
					_("Unconsistency in the INI file %1 at line #%2"), path, curr_no_line));
			}
			root[curr_section][key] = val;
		}

		// Cas autres
		else {
			throw std::runtime_error(Glib::ustring::compose(
				_("Unconsistency in the INI file %1 at line #%2"), path, curr_no_line));
		}
	}

	// Fermeture du fichier
	file.close();
}

// Écriture
void IniStruct::save(const std::string &path) const {

	// Ouverture du fichier cible
	std::ofstream file;
	file.open(path.c_str());
	if(file.fail()) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to open the INI file %1 for writing"), path));
	}

	// Boucle d'écriture
	for(Tree::const_iterator i=root.begin(); i!=root.end(); ++i) {
		assert(is_valid_key(i->first));
		file << "\n[" << i->first << "]" << std::endl;
		for(Section::const_iterator j=i->second.begin(); j!=i->second.end(); ++j) {
			assert(is_valid_key(j->first));
			file << j->first << "=" << j->second << std::endl;
		}
	}

	// Fermeture du fichier
	file.close();
}

// Test si une clé est valide, ie si elle est non-vide et ne contient que des
// caractères d'identifiant valides
bool IniStruct::is_valid_key(const Key &src) {
	if(src.empty())
		return false;
	for(size_t k=0; k<src.length(); ++k) {
		if(!is_valid_id_char(src.at(k)))
			return false;
	}
	return true;
}

// Test si un caractère est un caractère d'identifiant valide
bool IniStruct::is_valid_id_char(char src) {
	return (src>='A' && src<='Z') || (src>='a' && src<='z')
		|| (src>='0' && src<='9') || src=='_';
}

// Liste des sections
std::set<IniStruct::Key> IniStruct::sections() const {
	std::set<Key> res;
	for(Tree::const_iterator it=root.begin(); it!=root.end(); ++it) {
		res.insert(it->first);
	}
	return res;
}

// Liste des entrées dans une section
std::set<IniStruct::Key> IniStruct::codes(const Key &section) const {
	std::set<Key> res;
	Tree::const_iterator sec=root.find(section);
	if(sec==root.end())
		return res;
	for(Section::const_iterator it=sec->second.begin(); it!=sec->second.end(); ++it) {
		res.insert(it->first);
	}
	return res;
}

// Lecture d'un booléen
bool IniStruct::get_data(const Key &section, const Key &code, bool default_value) const
{
	std::string buff = get_data(section, code, default_value ? "true" : "false");
	if(buff=="true")
		return true;
	else if(buff=="false")
		return false;
	else
		return default_value;
}

// Lecture d'un entier
int IniStruct::get_data(const Key &section, const Key &code, int default_value) const
{
	std::string buff = get_data(section, code, int_to_string(default_value));
	if(is_valid_int(buff))
		return string_to_int(buff);
	else
		return default_value;
}

// Lecture d'une chaîne de caractères
std::string IniStruct::get_data(const Key &section, const Key &code,
	const char *default_value) const
{
	return get_data(section, code, std::string(default_value));
}
std::string IniStruct::get_data(const Key &section, const Key &code,
	const std::string &default_value) const
{
	Tree::const_iterator i=root.find(section);
	if(i==root.end())
		return default_value;
	else {
		Section::const_iterator j=i->second.find(code);
		if(j==i->second.end())
			return default_value;
		else
			return j->second;
	}
}

// Écriture d'un booléen
void IniStruct::set_data(const Key &section, const Key &code, bool value) {
	set_data(section, code, value ? "true" : "false");
}

// Écriture d'un entier
void IniStruct::set_data(const Key &section, const Key &code, int value) {
	set_data(section, code, int_to_string(value));
}

// Écriture d'un texte
void IniStruct::set_data(const Key &section, const Key &code, const char *value) {
	set_data(section, code, std::string(value));
}
void IniStruct::set_data(const Key &section, const Key &code, const std::string &value) {
	root[section][code] = value;
}

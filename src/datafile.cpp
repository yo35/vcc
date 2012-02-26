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


#include "datafile.h"
#include "strings.h"
#include <translation.h>
#include <stdexcept>
#include <cassert>
#include <glibmm/ustring.h>


////////////////////////////////////////////////////////////////////////////////
// Classe de base

DataFileBase::DataFileBase(const std::string &path) {
	m_path = path;
}

const std::string &DataFileBase::path() const {
	return m_path;
}


////////////////////////////////////////////////////////////////////////////////
// Classe de lecture

// Constructeur
DataFileIn::DataFileIn(const std::string &path) : DataFileBase(path) {}

// Ouverture du fichier
void DataFileIn::open() {
	if(is_open())
		return;
	m_file.open(path().c_str());
	if(m_file.fail()) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to open the data file %1 for reading"), path()));
	}
	m_curr_no_line = 0;
	m_eof          = false;
	m_buff.clear();
	parse_line();
}

// Fermeture du fichier
void DataFileIn::close() {
	if(!is_open())
		return;
	m_file.close();
}

// État d'ouverture
bool DataFileIn::is_open() const {
	return m_file.is_open();
}

// Test fin du fichier atteinte
bool DataFileIn::eof() const {
	assert(is_open());
	return m_eof;
}

// Lit un élément
int DataFileIn::get() {
	assert(is_open());
	check_consistency(!m_eof);
	assert(!m_buff.empty());
	int res = m_buff.front();
	m_buff.pop_front();
	if(m_buff.empty())
		parse_line();
	return res;
}

// Lecture d'une ligne
void DataFileIn::parse_line() {
	assert(is_open());

	// Trouve la première ligne non-vide qui ne soit pas un commentaire
	std::string curr_str_line;
	while(!m_file.eof()) {
		getline(m_file, curr_str_line);
		++m_curr_no_line;
		curr_str_line = trim(curr_str_line);
		if(!(curr_str_line.empty() || curr_str_line.at(0)=='#'))
			break;
	}
	if(m_file.eof() && curr_str_line.empty()) {
		m_eof = true;
		return;
	}

	// Suppression du commentaire en fin de ligne éventuel
	size_t pos_diese = curr_str_line.find('#');
	if(pos_diese!=std::string::npos) {
		curr_str_line = trim(curr_str_line.substr(0, pos_diese));
	}

	// Découpe la liste en fonction du séparateur ';'
	std::list<std::string> items = split(curr_str_line, ';');
	for(std::list<std::string>::const_iterator it=items.begin(); it!=items.end(); ++it) {
		std::string curr_item = trim(*it);
		if(curr_item=="")
			continue;
		check_consistency(is_valid_int(curr_item));
		m_buff.push_back(string_to_int(curr_item));
	}
}

// Routine de vérification (lance une exception si 'test' est faux)
void DataFileIn::check_consistency(bool test) const {
	if(!test) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unconsistency in the data file %1 at line #%2"), path(), m_curr_no_line));
	}
}



////////////////////////////////////////////////////////////////////////////////
// Classe d'écriture

// Constructeur
DataFileOut::DataFileOut(const std::string &path) : DataFileBase(path) {}

// Ouverture du fichier
void DataFileOut::open() {
	if(is_open())
		return;
	m_file.open(path().c_str());
	if(m_file.fail()) {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to open the data file %1 for writing"), path()));
	}
	m_cpt_items_on_line = 0;
}

// Fermeture du fichier
void DataFileOut::close() {
	if(!is_open())
		return;
	m_file.close();
}

// État d'ouverture
bool DataFileOut::is_open() const {
	return m_file.is_open();
}

// Écriture
void DataFileOut::put(int val) {
	assert(is_open());
	m_file << val << ";";
	++m_cpt_items_on_line;
	if(m_cpt_items_on_line==10) {
		m_file << std::endl;
		m_cpt_items_on_line = 0;
	}
}

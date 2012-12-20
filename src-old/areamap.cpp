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


#include "areamap.h"
#include "datafile.h"
#include <translation.h>
#include <cassert>
#include <stdexcept>
#include <glibmm/ustring.h>

// Constructeur
AreaMap::AreaMap() {}

// Nombre de touches
int  AreaMap::nb_keys() const {
	return m_data.size();
}

// Check si une touche est ou non affectée dans une région
bool AreaMap::is_affected(int key) const {
	assert(key>=0 && key<nb_keys());
	return m_data[key].affected;
}

// Si une touche est affectée dans une région, retourne cette région
Side AreaMap::side(int key) const {
	assert(is_affected(key));
	return m_data[key].side;
}

// Modifie le nombre de touches
void AreaMap::set_nb_keys(int src) {
	assert(src>=0);
	m_data.resize(src);
	clear();
}

// Modifie l'affectation d'une touche
void AreaMap::set_unaffected(int key) {
	assert(key>=0 && key<nb_keys());
	m_data[key].affected = false;
}

// Modifie l'affectation d'une touche
void AreaMap::set_side(int key, const Side &src) {
	assert(key>=0 && key<nb_keys());
	m_data[key].affected = true;
	m_data[key].side     = src ;
}

// Désaffectation de toutes les touches
void AreaMap::clear() {
	for(unsigned int idx=0; idx<m_data.size(); ++idx) {
		m_data[idx].affected = false;
	}
}

// Enregistrement dans le fichier sous-jacent
void AreaMap::save(const std::string &path) const {

	// Ouverture du fichier cible
	DataFileOut file(path);
	file.open();

	// Boucle d'enregistrement
	file.put(1            );
	file.put(m_data.size());
	for(unsigned idx=0; idx<m_data.size(); ++idx) {
		if(m_data[idx].affected)
			file.put(m_data[idx].side.to_int());
		else
			file.put(2);
	}

	// Terminaison
	file.close();
}

// Chargement depuis un fichier
void AreaMap::load(const std::string &path) {

	// Ouverture du fichier source
	DataFileIn file(path);
	file.open();

	// Numéro de version
	int no_version = file.get();
	if(no_version!=1) {
		throw std::runtime_error(Glib::ustring::compose(
			_("The version (%1) of the area map file %2 is not supported"), no_version, path));
	}

	// Nombre de touches
	int sz = file.get();
	if(sz<0) {
		throw std::runtime_error(Glib::ustring::compose(
			_("The number of keys reported the file %1 is incorrect"), path));
	}
	m_data.resize(sz);

	// Boucle de lecture
	for(unsigned int idx=0; idx<m_data.size(); ++idx) {
		int val = file.get();
		if(val==0 || val==1) {
			m_data[idx].affected = true;
			m_data[idx].side     = Side(val);
		}
		else if(val==2) {
			m_data[idx].affected = false;
		}
		else {
			throw std::runtime_error(Glib::ustring::compose(
				_("Incorrect affectation code for the key %1 in the file %2"), idx, path));
		}
	}

	// Terminaison
	file.close();
}

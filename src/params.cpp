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


#include "params.h"
#include <config.h>
#include <cassert>
#include <fstream>
#include <string>
#include <stdexcept>
#include <translation.h>
#include <giomm/file.h>
#include <glibmm/ustring.h>

Params *gp;

Params::Params(const std::string &prefix_path, const std::string &config_path) :
	m_prefix_path(prefix_path),
	m_config_path(config_path),
	m_vccini_path(config_path + "/" EXECUTABLE_NAME ".ini")
{
	// Création du répertoire de configuration dans le dossier de l'utilisateur
	// s'il n'existe pas déjà
	static Glib::RefPtr<Gio::File> config_dir = Gio::File::create_for_path(m_config_path);
	if(!config_dir->query_exists()) {
		config_dir->make_directory();
	}

	// Création d'un fichier de configuration vide s'il n'en existe pas un déjà
	static Glib::RefPtr<Gio::File> ini_file = Gio::File::create_for_path(m_vccini_path);
	if(!ini_file->query_exists()) {
		std::ofstream ini_file2;
		ini_file2.open(m_vccini_path.c_str());
		if(ini_file2.fail()) {
			throw std::runtime_error(Glib::ustring::compose(_(
				"Unable to create the INI configuration file at %1"), m_vccini_path));
		}
		ini_file2 << std::endl;
		ini_file2.close();
	}

	// Lecture du fichier INI
	m_data.load(m_vccini_path);

	// Zones actives
	KeyvalList    keyval_left ;
	KeyvalList    keyval_right;
	KeyvalList   *curr_area = 0;
	std::string   path = prefix_path + "/" + VCC_SHARE_RPATH  + "/keymap.txt";
	std::ifstream file;
	file.open(path.c_str());
	if(file.fail())
		throw std::runtime_error(_("Unable to open the keyboard configuration file"));
	while(!file.eof()) {
		std::string line;
		getline(file, line);

		// Cas d'une ligne vide
		if(line=="")
			continue;

		// Cas d'un changement de zone
		if(line=="LEFT")
			curr_area = &keyval_left;
		else if(line=="RIGHT")
			curr_area = &keyval_right;

		// Autre cas : on s'attend à un nombre
		else {
			int nb = 0;
			for(unsigned int k=0; k<line.length(); ++k) {
				if(line[k]<'0' || line[k]>'9')
					throw std::runtime_error(_("The keyboard configuration file is corrupted"));
				nb = nb*10 + line[k] - '0';
			}
			if(curr_area==0)
				throw std::runtime_error(_("The keyboard configuration file is corrupted"));
			curr_area->push_back(nb);
		}
	}
	file.close();
	init_kb_areas(keyval_left, keyval_right);
}

// Destructeur
Params::~Params() {
	m_data.save(m_vccini_path);
}

// Répertoire VCC_TOP
const std::string &Params::prefix_path() const {
	return m_prefix_path;
}

// Répertoire de configuration (dépend de l'utilisateur actif)
const std::string &Params::config_path() const {
	return m_config_path;
}

// Cadence de jeu initiale (lecture)
TimeControl Params::initial_time_control() const {
	TimeControl retval;
	retval.set_mode     (m_data.get_data("Time_Control", "Mode"           , SUDDEN_DEATH));
	retval.set_main_time(m_data.get_data("Time_Control", "Main_Time_Left" , 3*60*1000), LEFT );
	retval.set_main_time(m_data.get_data("Time_Control", "Main_Time_Right", 3*60*1000), RIGHT);
	if(retval.mode()==FISCHER || retval.mode()==BRONSTEIN) {
		retval.set_increment(m_data.get_data("Time_Control", "Increment_Left" , 2*1000), LEFT );
		retval.set_increment(m_data.get_data("Time_Control", "Increment_Right", 2*1000), RIGHT);
	}
	return retval;
}

// Cadence de jeu initiale (écriture)
void Params::set_initial_time_control(const TimeControl &src) {
	m_data.set_data("Time_Control", "Mode"           , src.mode());
	m_data.set_data("Time_Control", "Main_Time_Left" , src.main_time(LEFT ));
	m_data.set_data("Time_Control", "Main_Time_Right", src.main_time(RIGHT));
	if(src.mode()==FISCHER || src.mode()==BRONSTEIN) {
		m_data.set_data("Time_Control", "Increment_Left" , src.increment(LEFT ));
		m_data.set_data("Time_Control", "Increment_Right", src.increment(RIGHT));
	}
}

void Params::init_kb_areas(const KeyvalList &area_left, const KeyvalList &area_right) {

	// Chargement des listes de touches gauches et droites
	key_area[LEFT ] = aux_init_kb_areas(area_left );
	key_area[RIGHT] = aux_init_kb_areas(area_right);

	// On vérifie que l'intersection des deux ensembles est vide
	std::set<Keycode> area_both;
	area_both.insert(key_area[LEFT ].begin(), key_area[LEFT ].end());
	area_both.insert(key_area[RIGHT].begin(), key_area[RIGHT].end());
	assert(area_both.size()==key_area[LEFT].size()+key_area[RIGHT].size());
}

std::set<Keycode> Params::aux_init_kb_areas(const KeyvalList &src) {
	std::set<Keycode> retval;
	for(KeyvalList::const_iterator it=src.begin(); it!=src.end(); ++it) {
		KeycodeList keycodes = keyval_to_keycodes(*it);
		retval.insert(keycodes.begin(), keycodes.end());
	}
	return retval;
}

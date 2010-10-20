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


#ifndef PARAMS_H
#define PARAMS_H

#include "inistruct.h"
#include "timecontrol.h"
#include "keys.h"
#include <string>
#include <set>

class Params {
public:

	// Constructeur
	Params(const std::string &prefix_path);
	~Params();

	// Pointe vers le répertoire de base du package
	const std::string &prefix_path() const;

	// Cadence de jeu
	TimeControl initial_time_control() const;
	void set_initial_time_control(const TimeControl &src);

	// Zones actives pour les touches clavier
	EnumArray<Side, std::set<Keycode> > key_area;
	void init_kb_areas(const KeyvalList &area_left, const KeyvalList &area_right);

private:
	std::set<Keycode> aux_init_kb_areas(const KeyvalList &src);

	// Fonctions d'accès aux données (lecture)
	template<class T>
	Enumerable<T> get_data(const std::string &section, const std::string &key, const Enumerable<T> &default_value) const;
	int           get_data(const std::string &section, const std::string &key, int                  default_value) const;
	std::string   get_data(const std::string &section, const std::string &key, const std::string   &default_value) const;

	// Fonctions d'accès aux données (écriture)
	template<class T>
	void set_data(const std::string &section, const std::string &key, const Enumerable<T> &value);
	void set_data(const std::string &section, const std::string &key, int                  value);
	void set_data(const std::string &section, const std::string &key, const std::string   &value);

	// Conversion
	static std::string int_to_string(int src);
	static bool is_valid_int(const std::string &src, int *buff);

	// Données membres
	std::string m_prefix_path;
	IniStruct   m_data;
};

extern Params *gp;

#endif

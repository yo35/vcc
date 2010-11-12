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

#include "optionenums.h"
#include "inistruct.h"
#include "timecontrol.h"
#include "keys.h"
#include <string>
#include <set>

// Paramètres généraux
class Params {
public:

	// Constructeur
	Params(const std::string &prefix_path, const std::string &config_path);
	~Params();

	// Pointe vers les répertoires de base
	const std::string &prefix_path() const;
	const std::string &config_path() const;

	// Cadence de jeu
	TimeControl initial_time_control() const;
	void set_initial_time_control(const TimeControl &src);

	// Demande de confirmation pour la réinitialisation de l'horloge
	ReinitConfirm reinit_confirm() const;
	void set_reinit_confirm(const ReinitConfirm &src);

	// Touches à presser pour la remise à zéro
	KeyCombination reinit_keys() const;
	void set_reinit_key(const KeyCombination &src);

	// Délai avant réinitialisation pour la remise à zéro
	int reinit_delay() const;
	void set_reinit_delay(int src);

	// Zones actives pour les touches clavier
	EnumArray<Side, std::set<Keycode> > key_area;
	void init_kb_areas(const KeyvalList &area_left, const KeyvalList &area_right);

private:
	std::set<Keycode> aux_init_kb_areas(const KeyvalList &src);

	// Données membres
	const std::string m_prefix_path;
	const std::string m_config_path;
	const std::string m_vccini_path;
	const std::string m_kbmidx_path;
	IniStruct         m_data_perso;
	IniStruct         m_index_kbm;
};

extern Params *gp;

#endif

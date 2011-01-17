/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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
#include "keyboardmap.h"
#include "areamap.h"
#include <string>
#include <set>
#include <map>

// Paramètres généraux
class Params {
public:

	// Constructeur
	Params(const std::string &prefix_path, const std::string &config_path, const std::string &locale);
	~Params();

	// Locale courante
	const std::string &locale() const;

	// Pointe vers les répertoires de base
	const std::string &prefix_path() const;
	const std::string &config_path() const;

	// Premier démarrage
	bool first_launch() const;
	void set_first_launch(bool src);

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

	// Modèle de clavier préféré (code)
	std::string curr_keyboard() const;
	void set_curr_keyboard(const std::string &src);

	// Affichage du pavé numérique
	bool display_num_pad() const;
	void set_display_num_pad(bool src);

	// Zones préférées sur le clavier
	const AreaMap &kam_perso() const;
	void set_kam_perso(const AreaMap &src);

	// Modèles de claviers
	std::set<std::string> keyboards() const;
	Glib::ustring      keyboard_name   (const std::string &kbcode) const;
	const KeyboardMap &keyboard_map    (const std::string &kbcode) const;
	const AreaMap     &default_area_map(const std::string &kbcode) const;

private:

	// Code clavier à utiliser par défaut
	void init_default_kbm();

	// Données membres
	const std::string                          m_locale;
	const std::string                          m_prefix_path;
	const std::string                          m_config_path;
	const std::string                          m_vccini_path;
	const std::string                          m_my_kam_path;
	const std::string                          m_kbmidx_path;
	IniStruct                                  m_data_perso;
	IniStruct                                  m_index_kbm;
	std::string                                m_default_kbm;
	AreaMap                                    m_kam_perso;
	mutable std::map<std::string, KeyboardMap> m_proxy_kbm;
	mutable std::map<std::string, AreaMap    > m_proxy_kam;
};

extern Params *gp;

#endif

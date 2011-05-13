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


#include "params.h"
#include "strings.h"
#include <config.h>
#include <cassert>
#include <fstream>
#include <string>
#include <stdexcept>
#include <translation.h>
#include <giomm/file.h>
#include <glibmm/ustring.h>

Params *gp;

Params::Params(const std::string &prefix_path, const std::string &config_path,
	const std::string &locale) :
	m_locale     (locale     ),
	m_prefix_path(prefix_path),
	m_config_path(config_path),
	m_vccini_path(config_path + "/vcc.ini"),
	m_my_kam_path(config_path + "/vcc.kam"),
	m_kbmidx_path(prefix_path + "/" VCC_SHARE_RPATH "/keyboardmaps.ini")
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

	// Lecture du fichier INI perso
	m_data_perso.load(m_vccini_path);

	// Lecture de l'index des fichiers KBM
	m_index_kbm.load(m_kbmidx_path);

	// Code clavier à utiliser par défaut
	init_default_kbm();

	// Lecture du fichier .kam de l'utilisateur s'il existe
	// Sinon, on choisit par défaut le .kam associé au clavier courant
	static Glib::RefPtr<Gio::File> kam_perso_file = Gio::File::create_for_path(m_my_kam_path);
	if(kam_perso_file->query_exists()) {
		m_kam_perso.load(m_my_kam_path);
	}
	else {
		m_kam_perso = default_area_map(curr_keyboard());
	}
}

// Destructeur
Params::~Params() {
	m_data_perso.save(m_vccini_path);
	m_kam_perso.save (m_my_kam_path);
}

// Locale courante
const std::string &Params::locale() const {
	return m_locale;
}

// Répertoire VCC_TOP
const std::string &Params::prefix_path() const {
	return m_prefix_path;
}

// Répertoire de configuration (dépend de l'utilisateur actif)
const std::string &Params::config_path() const {
	return m_config_path;
}

// Premier démarrage (lecture)
bool Params::first_launch() const {
	return m_data_perso.get_data("Misc", "First_Launch", true);
}

// Premier démarrage (écriture)
void Params::set_first_launch(bool src) {
	m_data_perso.set_data("Misc", "First_Launch", src);
}

// Cadence de jeu initiale (lecture)
TimeControl Params::initial_time_control() const {
	TimeControl retval;
	retval.set_mode     (m_data_perso.get_data("Time_Control", "Mode"           , SUDDEN_DEATH));
	retval.set_main_time(m_data_perso.get_data("Time_Control", "Main_Time_Left" , 3*60*1000), LEFT );
	retval.set_main_time(m_data_perso.get_data("Time_Control", "Main_Time_Right", 3*60*1000), RIGHT);
	if(retval.mode()==FISCHER || retval.mode()==BRONSTEIN || retval.mode()==BYO_YOMI) {
		retval.set_increment(m_data_perso.get_data("Time_Control", "Increment_Left" , 2*1000), LEFT );
		retval.set_increment(m_data_perso.get_data("Time_Control", "Increment_Right", 2*1000), RIGHT);
	}
	if(retval.mode()==BYO_YOMI) {
		retval.set_byo_period(m_data_perso.get_data("Time_Control", "Byo_Period_Left" , 1), LEFT );
		retval.set_byo_period(m_data_perso.get_data("Time_Control", "Byo_Period_Right", 1), RIGHT);
	}
	return retval;
}

// Cadence de jeu initiale (écriture)
void Params::set_initial_time_control(const TimeControl &src) {
	m_data_perso.set_data("Time_Control", "Mode"           , src.mode());
	m_data_perso.set_data("Time_Control", "Main_Time_Left" , src.main_time(LEFT ));
	m_data_perso.set_data("Time_Control", "Main_Time_Right", src.main_time(RIGHT));
	if(src.mode()==FISCHER || src.mode()==BRONSTEIN || src.mode()==BYO_YOMI) {
		m_data_perso.set_data("Time_Control", "Increment_Left" , src.increment(LEFT ));
		m_data_perso.set_data("Time_Control", "Increment_Right", src.increment(RIGHT));
	}
	if(src.mode()==BYO_YOMI) {
		m_data_perso.set_data("Time_Control", "Byo_Period_Left" , src.byo_period(LEFT ));
		m_data_perso.set_data("Time_Control", "Byo_Period_Right", src.byo_period(RIGHT));
	}
}

// Demande de confirmation pour la réinitialisation de l'horloge (lecture)
ReinitConfirm Params::reinit_confirm() const {
	return m_data_perso.get_data("Reset_Options", "Confirm_When_Reinitializing", RC_IF_NOT_PAUSED);
}

// Demande de confirmation pour la réinitialisation de l'horloge (écriture)
void Params::set_reinit_confirm(const ReinitConfirm &src) {
	m_data_perso.set_data("Reset_Options", "Confirm_When_Reinitializing", src);
}

// Combinaison de touches pour la remise à zéro (lecture)
KeyCombination Params::reinit_keys() const {
	return m_data_perso.get_data("Reset_Options", "Key_Combination", DOUBLE_MAJ);
}

// Combinaison de touches pour la remise à zéro (écriture)
void Params::set_reinit_key(const KeyCombination &src) {
	m_data_perso.set_data("Reset_Options", "Key_Combination", src);
}

// Délai avant réinitialisation pour la remise à zéro (lecture)
int Params::reinit_delay() const {
	return m_data_perso.get_data("Reset_Options", "Delay", 1000);
}

// Délai avant réinitialisation pour la remise à zéro (écriture)
void Params::set_reinit_delay(int src) {
	m_data_perso.set_data("Reset_Options", "Delay", src);
}

// Affichage de la barre d'état (lecture)
bool Params::display_status_bar() const {
	return m_data_perso.get_data("Display", "Status_Bar", true);
}

// Affichage de la barre d'état (écriture)
void Params::set_display_status_bar(bool src) {
	m_data_perso.set_data("Display", "Status_Bar", src);
}

// Affichage du temps après la chute du drapeau (lecture)
bool Params::display_time_after_flag_down() const {
	return m_data_perso.get_data("Display", "Time_After_Flag_Down", true);
}

// Affichage du temps après la chute du drapeau (écriture)
void Params::set_display_time_after_flag_down(bool src) {
	m_data_perso.set_data("Display", "Time_After_Flag_Down", src);
}

// Affichage des informations additionnelles en mode 'Bronstein' (lecture)
bool Params::display_bronstein_extra_time() const {
	return m_data_perso.get_data("Display", "Bronstein_Mode_Extra_Information", true);
}

// Affichage des informations additionnelles en mode 'Bronstein' (écriture)
void Params::set_display_bronstein_extra_time(bool src) {
	m_data_perso.set_data("Display", "Bronstein_Mode_Extra_Information", src);
}

// Modèle de clavier préféré (code) (lecture)
std::string Params::curr_keyboard() const {
	return m_data_perso.get_data("Keyboard", "KBM", m_default_kbm);
}

// Modèle de clavier préféré (code) (écriture)
void Params::set_curr_keyboard(const std::string &src) {
	m_data_perso.set_data("Keyboard", "KBM", src);
}

// Affichage du pavé numérique (lecture)
bool Params::display_num_pad() const {
	return m_data_perso.get_data("Keyboard", "Numeric_Keypad", true);
}

// Affichage du pavé numérique (écriture)
void Params::set_display_num_pad(bool src) {
	m_data_perso.set_data("Keyboard", "Numeric_Keypad", src);
}

// Zones préférées sur le clavier (lecture)
const AreaMap &Params::kam_perso() const {
	return m_kam_perso;
}

// Zones préférées sur le clavier (lecture)
void Params::set_kam_perso(const AreaMap &src) {
	m_kam_perso = src;
}

// Liste des codes claviers disponibles
std::set<std::string> Params::keyboards() const {
	return m_index_kbm.sections();
}

// Nom d'un clavier
Glib::ustring Params::keyboard_name(const std::string &kbcode) const {
	std::string res = m_index_kbm.get_data(kbcode, "Name", "");
	if(res=="") {
		throw std::runtime_error(Glib::ustring::compose(
			_("Unable to retrieve the name of keyboard %1"), kbcode));
	}
	return res;
}

// Icone du clavier
Glib::RefPtr<Gdk::Pixbuf> Params::keyboard_icon(const std::string &kbcode) const {
	if(m_proxy_icon.find(kbcode)==m_proxy_icon.end()) {
		std::string icon_name = m_index_kbm.get_data(kbcode, "Icon", "");
		if(icon_name=="") {
			m_proxy_icon[kbcode] = Glib::RefPtr<Gdk::Pixbuf>();
		}
		else {
			m_proxy_icon[kbcode] = Gdk::Pixbuf::create_from_file(
				m_prefix_path + "/" VCC_SHARE_RPATH "/" + icon_name, 24, 24);
		}
	}
	return m_proxy_icon[kbcode];
}

// Plan de clavier
const KeyboardMap &Params::keyboard_map(const std::string &kbcode) const {
	if(m_proxy_kbm.find(kbcode)==m_proxy_kbm.end()) {
		std::string kbm_filename = m_index_kbm.get_data(kbcode, "File", "");
		if(kbm_filename=="") {
			throw std::runtime_error(Glib::ustring::compose(
				_("Unable to retrieve the name of the layout file for keyboard %1"), kbcode));
		}
		m_proxy_kbm[kbcode].load(m_prefix_path + "/" VCC_SHARE_RPATH "/" + kbm_filename);
	}
	return m_proxy_kbm[kbcode];
}

// Régions définies par défaut
const AreaMap &Params::default_area_map(const std::string &kbcode) const {
	if(m_proxy_kam.find(kbcode)==m_proxy_kam.end()) {
		std::string kam_filename = m_index_kbm.get_data(kbcode, "Default_areas", "");
		if(kam_filename=="") {
			throw std::runtime_error(Glib::ustring::compose(
				_("Unable to retrieve the name of the default area file for keyboard %1"), kbcode));
		}
		m_proxy_kam[kbcode].load(m_prefix_path + "/" VCC_SHARE_RPATH "/" + kam_filename);
	}
	return m_proxy_kam[kbcode];
}

// Code clavier à utiliser par défaut
void Params::init_default_kbm() {
	std::set<std::string> codes = m_index_kbm.sections();
	for(std::set<std::string>::const_iterator it=codes.begin(); it!=codes.end(); ++it) {
		std::list<std::string> curr_locales = split(m_index_kbm.get_data(*it, "Locales", ""), ',');
		for(std::list<std::string>::const_iterator k=curr_locales.begin(); k!=curr_locales.end(); ++k) {
			if(*k==m_locale) {
				m_default_kbm = *it;
				return;
			}
		}
	}
	m_default_kbm = "KBM_000";
}

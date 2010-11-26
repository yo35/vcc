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


#include "clockwindow.h"
#include "params.h"
#include "strings.h"
#include <config.h>
#include <translation.h>
#include <gtkmm/main.h>
#include <stdexcept>
#ifdef OS_IS_WINDOWS
	#include <windows.h>
#endif

#ifdef OS_IS_WINDOWS

	// Language conversion
	std::string convert_language(const std::string &src) {
		if(src=="English"   ) return "en";
		if(src=="French"    ) return "fr";
		if(src=="German"    ) return "de";
		if(src=="Swedish"   ) return "sv";
		if(src=="Portuguese") return "pt";
		if(src=="Italian"   ) return "it";
		return src;
	}
	
	// Country conversion
	std::string convert_country(const std::string &src) {
		if(src=="United States" ) return "US";
		if(src=="France"        ) return "FR";
		if(src=="Monaco"        ) return "MC";
		if(src=="Andorra"       ) return "AD";
		if(src=="Belgium"       ) return "BE";
		if(src=="Canada"        ) return "CA";
		if(src=="United Kingdom") return "UK";
		if(src=="Germany"       ) return "GE";
		if(src=="Austria"       ) return "AT";
		if(src=="Sweden"        ) return "SE";
		if(src=="Portugal"      ) return "PT";
		if(src=="Brazil"        ) return "BR";
		if(src=="Italy"         ) return "IT";
		return src;
	}

#endif

int main(int argc, char *argv[]) {

	// Initialisation GTK
	Gtk::Main kit(argc, argv);

	// Détermination du path de top-level
	#ifdef OS_IS_WINDOWS
		char buff[2048];
		if(GetModuleFileName(NULL, buff, 2048)==0)
			throw std::runtime_error("Unable to retrieve the filename of the executable");
		std::string exe_path = buff;
		std::string prefix_path = Glib::path_get_dirname(exe_path)
			+ "/" VCC_BIN_RPATH_BACKWARD;
	#else
		std::string prefix_path = VCC_TOP;
	#endif

	// Détermination du dossier de configuration
	#ifdef DEV_COMPILATION
		std::string config_path = VCC_TOP "/user_config";
	#else
		#ifdef OS_IS_WINDOWS
			std::string config_path = Glib::get_user_config_dir() + "/" PROJECT_NAME;
		#else
			std::string config_path = Glib::get_home_dir() + "/." PROJECT_NAME;
		#endif
	#endif

	// Initialisation libintl
	std::string locale_path = prefix_path + "/" VCC_LOCALE_RPATH;
	setlocale(LC_ALL, "");
	bindtextdomain(PROJECT_NAME, locale_path.c_str());
	bind_textdomain_codeset(PROJECT_NAME, "UTF-8");
	textdomain(PROJECT_NAME);

	// Détermination de la locale
	std::string locale = setlocale(LC_ALL, 0);
	size_t pos_sep = locale.find(".");
	if(pos_sep!=std::string::npos) {
		locale = locale.substr(0, pos_sep);
	}
	#ifdef OS_IS_WINDOWS
		pos_sep = locale.find("_");
		if(pos_sep!=std::string::npos) {
			std::string language_name = locale.substr(0, pos_sep);
			std::string country_name  = locale.substr(pos_sep+1);
			locale = convert_language(language_name) + "_" + convert_country(country_name);
		}
	#endif

	// Chargement des paramètres
	gp = new Params(prefix_path, config_path, locale);

	// Icône de l'application
	std::list<std::string> icon_sizes = split(ICON_SIZES, ';');
	std::list<Glib::RefPtr<Gdk::Pixbuf> > icons;
	for(std::list<std::string>::const_iterator it=icon_sizes.begin(); it!=icon_sizes.end(); ++it) {
		Glib::RefPtr<Gdk::Pixbuf> icon = Gdk::Pixbuf::create_from_file(
			prefix_path + "/" VCC_ICONS_RPATH "/" + *it + "/" ICON_NAME);
		icons.push_back(icon);
	}
	Gtk::Window::set_default_icon_list(icons);

	// GUI
	ClockWindow* mainUI = new ClockWindow();
	Gtk::Main::run(*mainUI);
	delete mainUI;

	// Terminaison du programme
	delete gp;
	return 0;
}

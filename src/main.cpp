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
#include <config.h>
#include <gtkmm/main.h>
#include <stdexcept>
#ifdef OS_IS_WINDOWS
	#include <windows.h>
#endif

int main(int argc, char *argv[]) {

	// Initialisation GTK
	Gtk::Main kit(argc, argv);

	// Chargement des paramètres
	#ifdef OS_IS_WINDOWS
		char buff[2048];
		if(GetModuleFileName(NULL, buff, 2048)==0)
			throw std::runtime_error("Unable to retrieve the filename of the executable");
		std::string exe_path = buff;
		std::string prefix_path = Glib::path_get_dirname(exe_path)
			+ "/" + VCC_BIN_RPATH_BACKWARD;
	#else
		std::string prefix_path = VCC_TOP;
	#endif
	gp = new Params(prefix_path);

	// GUI
	ClockWindow* mainUI = new ClockWindow();
	Gtk::Main::run(*mainUI);
	delete mainUI;

	// Terminaison du programme
	delete gp;
	return 0;
}

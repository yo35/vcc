################################################################################
#                                                                              #
#    This file is part of Virtual Chess Clock, a chess clock software          #
#                                                                              #
#    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>        #
#                                                                              #
#    This program is free software: you can redistribute it and/or modify      #
#    it under the terms of the GNU General Public License as published by      #
#    the Free Software Foundation, either version 3 of the License, or         #
#    (at your option) any later version.                                       #
#                                                                              #
#    This program is distributed in the hope that it will be useful,           #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of            #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
#    GNU General Public License for more details.                              #
#                                                                              #
#    You should have received a copy of the GNU General Public License         #
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.     #
#                                                                              #
################################################################################


# Create the project
project(virtual-chess-clock)


# Application name
set(APP_SHORT_NAME "vcc")
set(APP_NAME       "virtual-chess-clock")
set(APP_FULL_NAME  "Virtual Chess Clock")


# Application version
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 99)
set(APP_VERSION_PATCH 0)


# Directories - Development configuration
if(${INSTALL_TYPE} MATCHES Dev)
	set(PATH_TOP           ".")
	set(RPATH_BIN          "bin/${CMAKE_BUILD_TYPE}")
	set(RPATH_BIN_BACKWARD "../..")
	set(RPATH_SHARE        "data/share")
	set(RPATH_ICONS        "data/icons/unix")
	set(RPATH_APPLICATIONS "data/applications")
	set(RPATH_LOCALE       "locale")
	set(RPATH_DOC          ".")

# Directories - Release configuration
else(${INSTALL_TYPE} MATCHES Dev)
	set(PATH_TOP           ${CMAKE_INSTALL_PREFIX})
	set(RPATH_BIN          "bin")
	set(RPATH_BIN_BACKWARD "..")
	if(${WIN32})
		set(RPATH_SHARE        "share")
		set(RPATH_ICONS        "share/icons")
		set(RPATH_APPLICATIONS "share/applications")
		set(RAPTH_LOCALE       "share/locale")
		set(RPATH_DOC          ".")
	else()
		set(RPATH_SHARE        "share/${PROJECT_NAME}")
		set(RPATH_ICONS        "share/icons/hicolor")
		set(RPATH_APPLICATIONS "share/applications")
		set(RPATH_LOCALE       "share/locale")
		set(RPATH_DOC          "share/doc/${PROJECT_NAME}")
	endif()

endif()

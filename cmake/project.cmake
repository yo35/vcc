################################################################################
#                                                                              #
#    This file is part of Virtual Chess Clock, a chess clock software          #
#                                                                              #
#    Copyright (C) 2010-2014 Yoann Le Montagner <yo35(at)melix(dot)net>        #
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



################################################################################
# PROJECT DEFINITION
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


# Name of the executable program
set(EXECUTABLE_NAME "vcc")


# Development flag
if(${DEV})
	add_definitions(-DVCC_DEVELOPMENT_SETTINGS)
	message(STATUS "VCC development settings selected.")
endif()


# Directories - Development configuration
if(${DEV})
	set(SHARE_PATH       ${CMAKE_SOURCE_DIR}/share)
	set(CONFIG_PATH      ${CMAKE_BINARY_DIR}/user_config)
	set(TRANSLATION_PATH ${CMAKE_BINARY_DIR}/translation)
endif()




################################################################################
# SOURCE FILES
################################################################################

# C/CPP files
file(
	GLOB_RECURSE source_cpp_files RELATIVE ${CMAKE_SOURCE_DIR}
	src/*.cpp
)

# C/CPP header files
file(
	GLOB_RECURSE source_h_files RELATIVE ${CMAKE_SOURCE_DIR}
	src/*.h
)

# Configuration files
file(
	GLOB config_in_files RELATIVE ${CMAKE_SOURCE_DIR}
	config/*.h.in
)

# Translation files
file(
	GLOB translation_po_files RELATIVE ${CMAKE_SOURCE_DIR}
	translation/*.po
)

# Translation template file
set(translation_pot_file translation/vcc.pot)

# Windows resource file
if(${WIN32})
	set(resource_rc_file os-integration/windows/vcc.rc)
endif()

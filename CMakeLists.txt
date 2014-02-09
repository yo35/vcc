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
# Vérifications et configuration

# Check the CMake version
cmake_minimum_required(VERSION 2.8.11)


# Resource file (only for windows)
if(${WIN32})
	enable_language(RC)
	set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
endif()


include(cmake/project.cmake)
include(cmake/libraries.cmake)
include(cmake/compile.cmake)
include(cmake/translation.cmake)
include(cmake/translation-tools.cmake)
include(cmake/miscellaneous-tools.cmake)
include(cmake/install.cmake)






################################################################################
# Fichiers

# Fichiers source de l'exécutable

# Fichier de configuration

# Fichiers de données

# Fichier de ressource pour l'icône de l'application (uniquement sous windows)
if(${WIN32})
	set(icon_rc_file data/icons/windows/winicon.rc)
else()
	set(icon_rc_file "")
endif()

# Fichier desktop

# Fichiers README, licence, etc...
set(readme_aso_files README NEWS AUTHORS COPYING INSTALL ChangeLog)
get_filename_component("README_FULL_PATH" "README" ABSOLUTE)
get_filename_component("COPYING_FULL_PATH" "COPYING" ABSOLUTE)

# Ficher de template pour les traductions

# Fichiers de traduction



################################################################################
# Cibles annexes

# Génération du fichier ChangeLog
#add_custom_target(
#	changelog
#	COMMAND ./changelog.sh
#)


# # Installation
# 	# CPack
# 	if(${INSTALL_TYPE} MATCHES Package)
# 		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${PROJECT_VERSION_MAJOR}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.${PROJECT_VERSION_MINOR}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.${PROJECT_VERSION_PATCH}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
# 		set(CPACK_PACKAGE_DESCRIPTION_FILE ${README_FULL_PATH})
# 		include(CPack)
# 	endif()
# endif()

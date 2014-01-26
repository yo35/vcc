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


################################################################################
# Vérifications et configuration

# Check the CMake version
cmake_minimum_required(VERSION 2.8)


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





################################################################################
# Définition du projet

# Nom du projet

# Divers

# Noms complets

# Icône de l'application
file(GLOB RAW_ICON_SIZES data/icons/unix/*x*)
set(ICON_SIZES "")
foreach(loop_var ${RAW_ICON_SIZES})
	get_filename_component(tmp_var ${loop_var} NAME)
	set(ICON_SIZES ${ICON_SIZES} ${tmp_var})
endforeach(loop_var)
set(ICON_NAME "apps/vcc.png")



################################################################################
# Fichiers

# Fichiers source de l'exécutable

# Fichier de configuration

# Fichiers de données
file(
	GLOB
	data_shared_files
	data/share/*.png
	data/share/*.kbm
	data/share/*.kam
	data/share/*.ini
)

# Fichier de ressource pour l'icône de l'application (uniquement sous windows)
if(${WIN32})
	set(icon_rc_file data/icons/windows/winicon.rc)
else()
	set(icon_rc_file "")
endif()

# Fichier desktop
set(desktop_file data/applications/vcc.desktop)

# Fichiers README, licence, etc...
set(readme_aso_files README NEWS AUTHORS COPYING INSTALL ChangeLog)
get_filename_component("README_FULL_PATH" "README" ABSOLUTE)
get_filename_component("COPYING_FULL_PATH" "COPYING" ABSOLUTE)

# Ficher de template pour les traductions

# Fichiers de traduction




################################################################################
# Compilation

# Flags compilo et linker
if(${WIN32})
	add_definitions("-DOS_IS_WINDOWS")
	if(NOT (${INSTALL_TYPE} MATCHES Dev))
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
	endif(NOT (${INSTALL_TYPE} MATCHES Dev))
else()
	add_definitions("-DOS_IS_UNIX")
endif()

# Fichiers de configuration

# Exécutable



################################################################################
# Cibles annexes

# Supprimer tout ce qui n'est pas sur le SVN
add_custom_target(
	mrproper
	COMMAND ${CMAKE_COMMAND} -E remove -f cmake_*.cmake *.tar.gz *.deb *.exe
	COMMAND ${CMAKE_COMMAND} -E remove -f Makefile CMakeCache.txt install_manifest.txt
	COMMAND ${CMAKE_COMMAND} -E remove -f include/config.h
	COMMAND ${CMAKE_COMMAND} -E remove_directory bin
	COMMAND ${CMAKE_COMMAND} -E remove_directory locale
	COMMAND ${CMAKE_COMMAND} -E remove_directory CMakeFiles
	COMMAND ${CMAKE_COMMAND} -E remove_directory _CPack_Packages
	COMMAND ${CMAKE_COMMAND} -E remove_directory user_config
)

# Génération du fichier ChangeLog
add_custom_target(
	changelog
	COMMAND ./changelog.sh
)

# Affichage des statistiques
add_custom_target(
	stats
	COMMAND ./statistics.sh
)

# Génération du fichier de template pour les traductions

# Compilation des fichiers de traduction

# Informations

# Exécution

# # Installation
# else()
# 	message(STATUS "VCC is configured in ${INSTALL_TYPE} install mode")
# 	message(STATUS "VCC will be installed in ${CMAKE_INSTALL_PREFIX}")
# 
# 	# Cible 'make install'
# 	install(PROGRAMS ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_FULL_NAME}
# 		DESTINATION ${VCC_BIN_RPATH})
# 	if(${WIN32})
# 		install(FILES ${LIBRARY_OUTPUT_PATH}/${HOOKDLL_FULL_NAME}
# 			DESTINATION ${VCC_BIN_RPATH})
# 	endif()
# 	install(FILES ${data_shared_files} DESTINATION ${VCC_SHARE_RPATH}       )
# 	install(FILES ${readme_aso_files}  DESTINATION ${VCC_DOC_RPATH}         )
# 	install(FILES ${desktop_file}      DESTINATION ${VCC_APPLICATIONS_RPATH})
# 	foreach(loop_var ${ICON_SIZES})
# 		install(DIRECTORY data/icons/unix/${loop_var} DESTINATION ${VCC_ICONS_RPATH} PATTERN ".svn" EXCLUDE)
# 	endforeach(loop_var)
# 	foreach(loop_var ${translation_po_files})
# 		get_filename_component(INTL_CODE ${loop_var} NAME_WE)
# 		install(DIRECTORY locale/${INTL_CODE} DESTINATION ${VCC_LOCALE_RPATH})
# 	endforeach(loop_var)
# 
# 	# Libraries additionnelles pour windows
# 	if(${INSTALL_TYPE} MATCHES Package)
# 		if(${WIN32})
# 			install(DIRECTORY redist_win32/mingw32/         DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtkmm/           DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/atk/        DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/cairo/      DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/expat/      DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/fontconfig/ DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/freetype/   DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/glib/       DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/gtk+/       DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/gtk+-conf/  DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/libpng/     DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/pango/      DESTINATION "." PATTERN ".svn" EXCLUDE)
# 			install(DIRECTORY redist_win32/gtk+/zlib/       DESTINATION "." PATTERN ".svn" EXCLUDE)
# 		endif()
# 	endif()
# 
# 	# CPack
# 	if(${INSTALL_TYPE} MATCHES Package)
# 		if(${WIN32})
# 			set(CPACK_GENERATOR "NSIS")
# 		else()
# 			set(CPACK_GENERATOR "DEB")
# 		endif()
# 		set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
# 		set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
# 		set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
# 		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${PROJECT_VERSION_MAJOR}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.${PROJECT_VERSION_MINOR}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.${PROJECT_VERSION_PATCH}")
# 		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
# 		set(CPACK_PACKAGE_DESCRIPTION_FILE ${README_FULL_PATH})
# 		set(CPACK_RESOURCE_FILE_LICENSE ${COPYING_FULL_PATH})
# 		set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Virtual Chess Clock is a simple and free chess clock software")
# 		set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Yoann Le Montagner")
# 		set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtkmm-2.4-1c2a")
# 		set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
# 			"CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_FULL_NAME}.lnk' '$INSTDIR\\\\${VCC_BIN_RPATH}\\\\${EXECUTABLE_FULL_NAME}'")
# 		set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
# 			"Delete '$SMPROGRAMS\\\\$START_MENU\\\\${PROJECT_FULL_NAME}.lnk'")
# 		include(CPack)
# 	endif()
# endif()

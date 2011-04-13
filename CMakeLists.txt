################################################################################
#                                                                              #
#    This file is part of Virtual Chess Clock, a chess clock software          #
#                                                                              #
#    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>       #
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

# Version de CMake
cmake_minimum_required(VERSION 2.8)

# Mode d'optimisation
if(NOT (
	(${CMAKE_BUILD_TYPE} MATCHES Debug) OR
	(${CMAKE_BUILD_TYPE} MATCHES RelWithDebInfo) OR
	(${CMAKE_BUILD_TYPE} MATCHES Release))
)
	message(FATAL_ERROR "Must define CMAKE_BUILD_TYPE")
endif(NOT (
	(${CMAKE_BUILD_TYPE} MATCHES Debug) OR
	(${CMAKE_BUILD_TYPE} MATCHES RelWithDebInfo) OR
	(${CMAKE_BUILD_TYPE} MATCHES Release))
)

# Mode d'installation
if(NOT (
	(${INSTALL_TYPE} MATCHES Dev) OR
	(${INSTALL_TYPE} MATCHES Compile) OR
	(${INSTALL_TYPE} MATCHES Package))
)
	message(FATAL_ERROR "Must define INSTALL_TYPE")
endif(NOT (
	(${INSTALL_TYPE} MATCHES Dev) OR
	(${INSTALL_TYPE} MATCHES Compile) OR
	(${INSTALL_TYPE} MATCHES Package))
)

# Gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)

# Répertoires de sortie
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Fichiers ressource
if(${WIN32})
	enable_language(RC)
	set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -i <SOURCE> -o <OBJECT>")
endif(${WIN32})



################################################################################
# Définition du projet

# Nom du projet
project(virtual-chess-clock)

# Divers
set(PROJECT_FULL_NAME "Virtual Chess Clock")
set(EXECUTABLE_NAME "vcc")
set(HOOKDLL_NAME "winkeyhook")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 0)

# Noms complets
set(EXECUTABLE_FULL_NAME "${EXECUTABLE_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
set(HOOKDLL_FULL_NAME "${CMAKE_SHARED_LIBRARY_PREFIX}${HOOKDLL_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}")

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
file(
	GLOB
	src_cpp_files
	src/*.cpp
)
file(
	GLOB
	src_h_files
	src/*.h
)

# Fichier de configuration
set(config_in_file include/config.h.in)
set(config_h_file include/config.h)

# Fichiers source de la DLL annexe
file(
	GLOB
	dll_c_files
	src/winkeyhook/*.c
)

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
else(${WIN32})
	set(icon_rc_file "")
endif(${WIN32})

# Fichier desktop
set(desktop_file data/applications/vcc.desktop)

# Fichiers README, licence, etc...
set(readme_aso_files README NEWS AUTHORS COPYING INSTALL ChangeLog)
get_filename_component("README_FULL_PATH" "README" ABSOLUTE)
get_filename_component("COPYING_FULL_PATH" "COPYING" ABSOLUTE)

# Ficher de template pour les traductions
set(template_pot_file po/vcc.pot)

# Fichiers de traduction
file(
	GLOB
	translation_po_files
	po/*.po
)



################################################################################
# Répertoires en runtime

# Mode debug
if(${INSTALL_TYPE} MATCHES Dev)
	set(VCC_TOP ".")
	set(VCC_BIN_RPATH "bin/${CMAKE_BUILD_TYPE}")
	set(VCC_BIN_RPATH_BACKWARD "../..")
	set(VCC_SHARE_RPATH "data/share")
	set(VCC_ICONS_RPATH "data/icons/unix")
	set(VCC_APPLICATIONS_RPATH "data/applications")
	set(VCC_DOC_RPATH ".")
	set(VCC_LOCALE_RPATH "locale")

# Mode release
else(${INSTALL_TYPE} MATCHES Dev)
	set(VCC_TOP ${CMAKE_INSTALL_PREFIX})
	set(VCC_BIN_RPATH "bin")
	set(VCC_BIN_RPATH_BACKWARD "..")
	if(${WIN32})
		set(VCC_SHARE_RPATH "share")
		set(VCC_ICONS_RPATH "share/icons")
		set(VCC_APPLICATIONS_RPATH "share/applications")
		set(VCC_DOC_RPATH ".")
		set(VCC_LOCALE_RPATH "share/locale")
	else(${WIN32})
		set(VCC_SHARE_RPATH "share/${PROJECT_NAME}")
		set(VCC_ICONS_RPATH "share/icons/hicolor")
		set(VCC_APPLICATIONS_RPATH "share/applications")
		set(VCC_DOC_RPATH "share/doc/${PROJECT_NAME}")
		set(VCC_LOCALE_RPATH "share/locale")
	endif(${WIN32})

endif(${INSTALL_TYPE} MATCHES Dev)



################################################################################
# Compilation

# Flags compilo et linker
add_definitions("-pedantic -Wall -Wextra")
include_directories(${GTKMM_INCLUDE_DIRS} include)
link_directories(${GTKMM_LIBRARY_DIRS})
if(${WIN32})
	add_definitions("-DOS_IS_WINDOWS")
	if(NOT (${INSTALL_TYPE} MATCHES Dev))
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
	endif(NOT (${INSTALL_TYPE} MATCHES Dev))
endif(${WIN32})
if(${INSTALL_TYPE} MATCHES Dev)
	add_definitions("-DDEV_COMPILATION")
endif(${INSTALL_TYPE} MATCHES Dev)

# Fichiers de configuration
configure_file(
	${config_in_file}
	${config_h_file}
)

# Bibliothèque pour la désactivation de la touche windows
if(${WIN32})
	add_library(
		${HOOKDLL_NAME}
		SHARED
		${dll_c_files}
	)
	set(HOOKDLL_LIBRARY ${HOOKDLL_NAME})
else(${WIN32})
	set(HOOKDLL_LIBRARY "")
endif(${WIN32})

# Exécutable
add_executable(
	${EXECUTABLE_NAME}
	${src_cpp_files}
	${src_h_files}
	${icon_rc_file}
)
target_link_libraries(
	${EXECUTABLE_NAME}
	${GTKMM_LIBRARIES}
	${HOOKDLL_LIBRARY}
)



################################################################################
# Cibles annexes

# Supprimer tout ce qui n'est pas sur le SVN
add_custom_target(
	mrproper
	COMMAND ${CMAKE_COMMAND} -E remove -f *.cmake *.tar.gz *.deb *.exe
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
add_custom_target(
	pot
	COMMAND xgettext --from-code=UTF-8 --keyword=_ --package-name=${PROJECT_NAME}
		--package-version=${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
		--msgid-bugs-address='Yoann Le Montagner <yo35(at)melix(dot)net>'
		--copyright-holder='Yoann Le Montagner <yo35(at)melix(dot)net>'
		-o ${template_pot_file} ${src_cpp_files} ${src_h_files} ${dll_c_files}
)

# Compilation des fichiers de traduction
set(TRANSLATIONS_DEPENDS "")
foreach(loop_var ${translation_po_files})
	get_filename_component(INTL_CODE ${loop_var} NAME_WE)
	add_custom_command(
		OUTPUT locale/${INTL_CODE}/LC_MESSAGES/${PROJECT_NAME}.mo
		COMMAND ${CMAKE_COMMAND} -E make_directory locale/${INTL_CODE}/LC_MESSAGES
		COMMAND msgfmt -c -v -o $@ $<
		DEPENDS ${loop_var}
	)
	set(TRANSLATIONS_DEPENDS ${TRANSLATIONS_DEPENDS} locale/${INTL_CODE}/LC_MESSAGES/${PROJECT_NAME}.mo)
endforeach(loop_var)
add_custom_target(
	translations
	DEPENDS ${TRANSLATIONS_DEPENDS}
)
add_dependencies(${EXECUTABLE_NAME} translations)

# Informations
message(STATUS "VCC will be build in ${CMAKE_BUILD_TYPE} mode")

# Exécution
if(${INSTALL_TYPE} MATCHES Dev)
	message(STATUS "VCC is configured for devel")
	add_custom_target(
		run
		COMMAND ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_NAME}
		DEPENDS ${EXECUTABLE_NAME}
	)

# Installation
else(${INSTALL_TYPE} MATCHES Dev)
	message(STATUS "VCC is configured in ${INSTALL_TYPE} install mode")
	message(STATUS "VCC will be installed in ${CMAKE_INSTALL_PREFIX}")

	# Cible 'make install'
	install(PROGRAMS ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_FULL_NAME}
		DESTINATION ${VCC_BIN_RPATH})
	if(${WIN32})
		install(FILES ${LIBRARY_OUTPUT_PATH}/${HOOKDLL_FULL_NAME}
			DESTINATION ${VCC_BIN_RPATH})
	endif(${WIN32})
	install(FILES ${data_shared_files} DESTINATION ${VCC_SHARE_RPATH}       )
	install(FILES ${readme_aso_files}  DESTINATION ${VCC_DOC_RPATH}         )
	install(FILES ${desktop_file}      DESTINATION ${VCC_APPLICATIONS_RPATH})
	foreach(loop_var ${ICON_SIZES})
		install(DIRECTORY data/icons/unix/${loop_var} DESTINATION ${VCC_ICONS_RPATH} PATTERN ".svn" EXCLUDE)
	endforeach(loop_var)
	foreach(loop_var ${translation_po_files})
		get_filename_component(INTL_CODE ${loop_var} NAME_WE)
		install(DIRECTORY locale/${INTL_CODE} DESTINATION ${VCC_LOCALE_RPATH})
	endforeach(loop_var)

	# Libraries additionnelles pour windows
	if(${INSTALL_TYPE} MATCHES Package)
		if(${WIN32})
			install(DIRECTORY redist_win32/mingw32/         DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtkmm/           DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/atk/        DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/cairo/      DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/expat/      DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/fontconfig/ DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/freetype/   DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/glib/       DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/gtk+/       DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/gtk+-conf/  DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/libpng/     DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/pango/      DESTINATION "." PATTERN ".svn" EXCLUDE)
			install(DIRECTORY redist_win32/gtk+/zlib/       DESTINATION "." PATTERN ".svn" EXCLUDE)
		endif(${WIN32})
	endif(${INSTALL_TYPE} MATCHES Package)

	# CPack
	if(${INSTALL_TYPE} MATCHES Package)
		if(${WIN32})
			set(CPACK_GENERATOR "NSIS")
		else(${WIN32})
			set(CPACK_GENERATOR "DEB")
		endif(${WIN32})
		set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
		set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
		set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${PROJECT_VERSION_MAJOR}")
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.${PROJECT_VERSION_MINOR}")
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}.${PROJECT_VERSION_PATCH}")
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
		set(CPACK_PACKAGE_DESCRIPTION_FILE ${README_FULL_PATH})
		set(CPACK_RESOURCE_FILE_LICENSE ${COPYING_FULL_PATH})
		set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Virtual Chess Clock is a simple and free chess clock software")
		set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Yoann Le Montagner")
		set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtkmm-2.4-1c2a")
		set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
			"CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_FULL_NAME}.lnk' '$INSTDIR\\\\${VCC_BIN_RPATH}\\\\${EXECUTABLE_FULL_NAME}'")
		set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
			"Delete '$SMPROGRAMS\\\\$START_MENU\\\\${PROJECT_FULL_NAME}.lnk'")
		include(CPack)
	endif(${INSTALL_TYPE} MATCHES Package)
endif(${INSTALL_TYPE} MATCHES Dev)

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
# UTILITIES TO MANAGE THE TEXT TRANSLATION FILES
################################################################################


# Find the GNU Gettext compilation tool
# If it cannot be found, the built of the executable is not impact at all, but the translation management tools
# (such as automatic extraction of the "to be translated" string in the source files)
# will not be available.
find_package(Gettext)
if(NOT ${GETTEXT_FOUND})
	message(STATUS "Cannot find Gettext: translation management tools will not be available.")
	return()
endif()
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext) # Why not done by default by find_package(Gettext)? 


# Custom target to generate or update the translation template file .pot
add_custom_target(
	translation-template-pot
	COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE}
		--from-code=UTF-8
		--keyword=_
		--package-name=${APP_NAME}
		--package-version=${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}
		-o ${translation_pot_file}
		${source_cpp_files} ${source_h_files}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)


# Custom target to update all the translation files .po
add_custom_target(
	translation-templates
	DEPENDS translation-template-pot
)


# Custom targets to update the translation files .po
foreach(translation_po_file ${translation_po_files})
	get_filename_component(locale_code ${translation_po_file} NAME_WE)
	add_custom_target(
		"translation-template-po-${locale_code}"
		COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE}
			--backup=off
			-U ${translation_po_file}
			${translation_pot_file}
		DEPENDS translation-template-pot
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	add_dependencies(translation-templates "translation-template-po-${locale_code}")
endforeach()

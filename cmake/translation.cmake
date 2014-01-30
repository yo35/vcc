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
# GENERATION OF THE BINARY TRANSLATION FILES
################################################################################


# Find the QT compilation tool (lconvert)
# If it cannot be found, the executable is still built, but not localized:
# most of the texts will appear in English.
find_package(Qt5LinguistTools)
if(NOT ${Qt5LinguistTools_FOUND})
	message(WARNING "Cannot find Qt5 linguist tools: translation will not be available.")
	return()
endif()
find_program(Qt5_LCONVERT_EXECUTABLE lconvert
	PATHS ${Qt5LinguistTools_DIR}/../../../bin
	NO_DEFAULT_PATH
)


# Compilation of text files into QT's .qm files
set(translation_qm_files "")
foreach(file_in ${translation_po_files})
	string(REGEX REPLACE "\\.po$" ".qm" file_out ${file_in})
	get_filename_component(out_directory ${CMAKE_BINARY_DIR}/${file_out} PATH)
	add_custom_command(
		OUTPUT ${file_out}
		COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${out_directory}
		COMMAND ${Qt5_LCONVERT_EXECUTABLE} ARGS -o ${file_out} ${CMAKE_SOURCE_DIR}/${file_in}
		DEPENDS ${file_in}
	)
	set(translation_qm_files ${translation_qm_files} ${file_out})
endforeach()


# Add a target 'translations' to compile all the translation text files
add_custom_target(translations DEPENDS ${translation_qm_files})


# Make the executable depends on the 'translations' target
add_dependencies(${EXECUTABLE_NAME} translations)

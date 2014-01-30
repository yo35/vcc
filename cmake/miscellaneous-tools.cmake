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
# VARIOUS GENERAL-PURPOSE TOOLS AND COMMANDS
################################################################################


# Compile and run the software (only available in the development configuration)
#  -> target 'run'
if(${DEV})
	add_custom_target(
		run
		COMMAND ./${EXECUTABLE_NAME}
		DEPENDS ${EXECUTABLE_NAME}
	)
endif()


# Code metrics (only available on Unix)
#  -> target 'stats'
if(${UNIX})
	add_custom_target(
		stats
		COMMAND ./statistics.sh ${source_cpp_files} ${source_h_files}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
endif()

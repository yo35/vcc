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
# GENERATION OF THE EXECUTABLE AND RELATED INSTRUCTIONS
################################################################################


# Include directories (for .h files)
include_directories(
	${all_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/config
	${CMAKE_SOURCE_DIR}/src
)


# Link directories (for library files)
link_directories(
	${all_LIBRARY_DIRS}
)


# Enable all the warnings
add_definitions(-pedantic -Wall -Wextra)


# Enable C++11 features
add_definitions(-std=c++11)
# Remark for the "Eclipse CDT4" generators: to make C++11 code recognize by Eclipse,
# it may be necessary to define manually the preprocessor symbol __GXX_EXPERIMENTAL_CXX0X__
# in the project settings.


# OS-detection flags
if(${WIN32})
	add_definitions(-DOS_IS_WINDOWS)
endif()
if(${UNIX})
	add_definitions(-DOS_IS_UNIX)
endif()


# Generate the configuration files
set(config_h_files "")
foreach(file_in ${config_in_files})
	string(REGEX REPLACE "\\.in$" "" file_out ${file_in})
	configure_file(${file_in} ${file_out})
	set(config_h_files ${config_h_files} ${file_out})
endforeach()


# Compile the executable and link it to the required libraries
add_executable(
	${EXECUTABLE_NAME}
	${source_cpp_files}
)
target_link_libraries(
	${EXECUTABLE_NAME}
	${all_LIBRARIES}
)


# QT's special compilation instructions
qt5_use_modules(
	${EXECUTABLE_NAME}
	${Qt5Modules}
)

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
# THIRD PARTY LIBRARIES USED BY THE PROGRAM
################################################################################


# Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system filesystem regex)


# Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
set(Qt5Modules Widgets)
if(${UNIX})
	find_package(Qt5X11Extras REQUIRED)
	set(Qt5Modules ${Qt5Modules} X11Extras)
endif()


# XCB
if(${UNIX})
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(Xcb REQUIRED xcb)
endif()


# All libraries together
set(all_INCLUDE_DIRS
	${Boost_INCLUDE_DIRS} ${Xcb_INCLUDE_DIRS}
)
set(all_LIBRARY_DIRS
	${Boost_LIBRARY_DIRS} ${Xcb_LIBRARY_DIRS}
)
set(all_LIBRARIES
	${Boost_LIBRARIES} ${Xcb_LIBRARIES}
)

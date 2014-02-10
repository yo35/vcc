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
# INSTALLATION INSTRUCTIONS
################################################################################

# No install-related target is provided in the development configuration.
if(${DEV})
	return()
endif()


# Show the install directory to the user
message(STATUS "VCC will be installed to ${CMAKE_INSTALL_PREFIX}")


# Define the destination folders.
if(${WIN32})
	set(DESTINATION_DIRECTORY_BIN ".")
	set(DESTINATION_DIRECTORY_SHARE share)
elseif(${UNIX})
	set(DESTINATION_DIRECTORY_BIN bin)
	set(DESTINATION_DIRECTORY_SHARE share/${APP_NAME})
endif()
set(DESTINATION_DIRECTORY_TRANSLATION ${DESTINATION_DIRECTORY_SHARE}/translation)


# Install instructions for the executable and for the required associated files.
install(TARGETS ${EXECUTABLE_NAME}
	RUNTIME DESTINATION ${DESTINATION_DIRECTORY_BIN}
)
install(DIRECTORY share/
	DESTINATION ${DESTINATION_DIRECTORY_SHARE}
)


# Install instructions for the translation files, if available.
if(DEFINED translation_qm_files)
	install(FILES ${translation_qm_files}
		DESTINATION ${DESTINATION_DIRECTORY_TRANSLATION}
	)
endif()


# OS integration specific instructions -> Unix.
if(${UNIX})
	install(DIRECTORY os-integration/unix/
		DESTINATION share
	)
endif()


# OS integration specific instructions -> Windows.
if(${WIN32})

	# GCC-specific libraries
	find_library(RuntimeLib_GCC_S_DW2_1  "gcc_s_dw2-1")
	find_library(RuntimeLib_STDC_PP_6    "stdc++-6")
	find_library(RuntimeLib_WINPTHREAD_1 "winpthread-1")
	set(RuntimeLib_GCC_ALL
		${RuntimeLib_GCC_S_DW2_1}
		${RuntimeLib_STDC_PP_6}
		${RuntimeLib_WINPTHREAD_1}
	)

	# Qt-specific libraries
	# TODO: How to automatically detect the .dll needed by Qt?
	find_library(RuntimeLib_QT5CORE    "Qt5Core")
	find_library(RuntimeLib_QT5GUI     "Qt5Gui")
	find_library(RuntimeLib_QT5WIDGETS "Qt5Widgets")
	find_library(RuntimeLib_ICUDT51    "icudt51")
	find_library(RuntimeLib_ICUIN51    "icuin51")
	find_library(RuntimeLib_ICUUC51    "icuuc51")
	set(RuntimeLib_QT_ALL
		${RuntimeLib_QT5CORE}
		${RuntimeLib_QT5GUI}
		${RuntimeLib_QT5WIDGETS}
		${RuntimeLib_ICUDT51}
		${RuntimeLib_ICUIN51}
		${RuntimeLib_ICUUC51}
	)

	# Qt-specific plugin libraries
	find_library(RuntimeLib_QWINDOWS "qwindows" PATH_SUFFIXES ../plugins/platforms)

	# Install instructions for the runtime libraries
	install(FILES ${RuntimeLib_GCC_ALL} ${RuntimeLib_QT_ALL}
		DESTINATION ${DESTINATION_DIRECTORY_BIN}
	)
	install(FILES ${RuntimeLib_QWINDOWS}
		DESTINATION ${DESTINATION_DIRECTORY_BIN}/platforms
	)

endif()



################################################################################
# PACKAGE GENERATION
################################################################################

# Select the package generator.
if(${WIN32})
	set(CPACK_GENERATOR "NSIS")
endif()
if(${UNIX})
	set(CPACK_GENERATOR "DEB")
endif()


# General package description variables.
set(CPACK_PACKAGE_NAME ${APP_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${APP_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${APP_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${APP_VERSION_PATCH})


# Describe the package.
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Virtual Chess Clock is a simple and free chess clock software")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)


# NSIS-installer associated variables
if(${WIN32})
	set(CPACK_PACKAGE_INSTALL_DIRECTORY ${APP_NAME})
	set(CPACK_NSIS_PACKAGE_NAME ${APP_FULL_NAME})
	set(CPACK_NSIS_MENU_LINKS
		${DESTINATION_DIRECTORY_BIN}/${EXECUTABLE_NAME} ${APP_FULL_NAME}
	)
endif()


# Debian-package associated variables.
if(${UNIX})
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Yoann Le Montagner <yo35@melix.net>")
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libqt5widgets5")
endif()


# Call the CPack generator.
if(DEFINED CPACK_GENERATOR)
	include(CPack)
endif()

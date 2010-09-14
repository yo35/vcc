
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
if(NOT DEFINED COMPILE_FOR_DEV)
	message(FATAL_ERROR "Must define COMPILE_FOR_DEV")
endif(NOT DEFINED COMPILE_FOR_DEV)

# Gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)

# Répertoires de sortie
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})



################################################################################
# Définition du projet

# Nom du projet
project(virtual-chess-clock)

# Divers
set(PROJECT_FULL_NAME "Virtual Chess Clock")
set(EXECUTABLE_NAME "vcc")
set(HOOKDLL_NAME "winkeyhook")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 5)



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

# Icônes internes
file(
	GLOB
	icon_int_files
	data/*.svg
)

# Fichier de keymap
set(keymap_file data/keymap.txt)



################################################################################
# Répertoires en runtime

# Mode debug
if(${COMPILE_FOR_DEV})
	set(VCC_BASE_PATH ".")
	set(VCC_SHARE_RPATH "data")
	set(VCC_BIN_RPATH "bin")

# Mode release
else(${COMPILE_FOR_DEV})
	set(VCC_BASE_PATH ${CMAKE_INSTALL_PREFIX})
	set(VCC_SHARE_RPATH "share/${PROJECT_NAME}")
	set(VCC_BIN_RPATH "bin")

endif(${COMPILE_FOR_DEV})



################################################################################
# Compilation

# Flags compilo et linker
add_definitions("-pedantic -Wall -Wextra")
include_directories(${GTKMM_INCLUDE_DIRS} include)
link_directories(${GTKMM_LIBRARY_DIRS})
if(${WIN32})
	add_definitions("-DOS_IS_WINDOWS")
	if(${COMPILE_FOR_DEV})
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
	endif(${COMPILE_FOR_DEV})
endif(${WIN32})

# Fichier de configuration
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
	COMMAND ${CMAKE_COMMAND} -E remove -f Makefile CMakeCache.txt cmake_install.cmake
	COMMAND ${CMAKE_COMMAND} -E remove -f install_manifest.txt include/config.h
	COMMAND ${CMAKE_COMMAND} -E remove_directory bin
	COMMAND ${CMAKE_COMMAND} -E remove_directory CMakeFiles
)

# Génération du fichier ChangeLog
find_program(SVN2CL svn2cl)
if(NOT (${SVN2CL} MATCHES "SVN2CL-NOTFOUND"))
	add_custom_target(
		changelog
		COMMAND ${SVN2CL} --group-by-day
		COMMAND ./process-changelog.sh
	)
endif(NOT (${SVN2CL} MATCHES "SVN2CL-NOTFOUND"))

# Exécution
if(${COMPILE_FOR_DEV})
	add_custom_target(
		run
		COMMAND ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_NAME}
		DEPENDS ${EXECUTABLE_NAME}
	)

# Installation
else(${COMPILE_FOR_DEV})
	INSTALL(PROGRAMS ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_NAME} DESTINATION ${VCC_BIN_RPATH})
	INSTALL(FILES ${icon_int_files} DESTINATION ${VCC_SHARE_RPATH})
	INSTALL(FILES ${keymap_file} DESTINATION ${VCC_SHARE_RPATH})

endif(${COMPILE_FOR_DEV})

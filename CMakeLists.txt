
################################################################################
# Vérifications et configuration

# Version de CMake
cmake_minimum_required(VERSION 2.8)

# Mode de compilation
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

# Gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)

# Répertoires de sorite
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})



################################################################################
# Définition du projet

# Généralités
project(ChessClock)
set(EXECUTABLE_NAME "chess-clock")
set(HOOKDLL_NAME "winkeyhook")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 5)

# Liste des fichiers sources
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
file(
	GLOB
	dll_c_files
	src/winkeyhook/*.c
)

# Divers
add_definitions("-pedantic -Wall -Wextra")
include_directories(${GTKMM_INCLUDE_DIRS} include)
link_directories(${GTKMM_LIBRARY_DIRS})
if(${WIN32})
	add_definitions("-DOS_IS_WINDOWS")
endif(${WIN32})
if(${WIN32} AND (${CMAKE_BUILD_TYPE} MATCHES Release))
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif(${WIN32} AND (${CMAKE_BUILD_TYPE} MATCHES Release))

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

# make run
add_custom_target(
	run
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE_NAME}
	DEPENDS ${EXECUTABLE_NAME}
)

# make mrproper
add_custom_target(
	mrproper
	COMMAND ${CMAKE_COMMAND} -E remove -f Makefile CMakeCache.txt cmake_install.cmake
	COMMAND ${CMAKE_COMMAND} -E remove_directory bin
	COMMAND ${CMAKE_COMMAND} -E remove_directory CMakeFiles
)

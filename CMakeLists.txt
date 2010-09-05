
################################################################################
# Vérifications

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




################################################################################
# Définition du projet

# Généralités
project(ChessClock)
set(EXECUTABLE_NAME "chess-clock")
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

# Divers
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_definitions("-pedantic -Wall -Wextra")
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

# Exécutable
add_executable(
	chess-clock
	${src_cpp_files}
	${src_h_files}
)
target_link_libraries(
	chess-clock
	${GTKMM_LIBRARIES}
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

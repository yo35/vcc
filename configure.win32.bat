@echo off

rem Avertissement
echo This script is only a frontend for cmake. You can also run cmake directly.

rem Texte d'aide
goto end_help
:begin_help
	echo Use: ./configure.win32 [options]
	echo Available options are:
	echo   --prefix=PATH : top directory where VCC will be installed
	echo   --install_type=[Dev or Compile or Package] : installation mode:
	echo      * Dev : no installation ; provide a target 'make run' to run a development version of VCC from the current directory
	echo      * Compile : installation through the usual process 'configure, make, make install'
	echo      * Package : provide a target 'make package' to generate the installation packages
	echo   --build_type=[Debug or RelWithDebInfo or Release] : optimization level:
	echo      * Debug : compilation with -O0
	echo      * RelWithDebInfo : compilation with -O2
	echo      * Release : compilation with -O3 -DNDEBUG
	echo Example: configure.win32 --build_type=RelWithDebInfo --install_type=Dev
	goto end_of_script
:end_help

rem Paramètres par défaut
set CMAKE_INSTALL_PREFIX="c:\Program Files\virtual-chess-clock"
set INSTALL_TYPE=Compile
set CMAKE_BUILD_TYPE=Release

rem Analyse de la ligne de commande
:loop_parse_args
	if %1""=="" goto end_loop_parse_args
	if %1==--prefix goto read_param_prefix
	if %1==--install_type goto read_param_install
	if %1==--build_type goto read_param_build
	if %1==--help goto begin_help
	:end_read_param
	shift
	goto loop_parse_args
	
	:read_param_prefix
		shift
		set CMAKE_INSTALL_PREFIX=%1
		goto end_read_param
	:read_param_install
		shift
		set INSTALL_TYPE=%1
		goto end_read_param
	:read_param_build
		shift
		set CMAKE_BUILD_TYPE=%1
		goto end_read_param
	
:end_loop_parse_args

rem Appel à cmake
set OPTS=-G"MinGW Makefiles"
set OPTS=%OPTS% -DCMAKE_INSTALL_PREFIX=%CMAKE_INSTALL_PREFIX%
set OPTS=%OPTS% -DINSTALL_TYPE=%INSTALL_TYPE%
set OPTS=%OPTS% -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE%
cmake %OPTS%

rem Point de sortie du script
:end_of_script

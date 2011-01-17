@echo off
rem ############################################################################
rem #                                                                          #
rem #    This file is part of Virtual Chess Clock, a chess clock software      #
rem #                                                                          #
rem #    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>   #
rem #                                                                          #
rem #    This program is free software: you can redistribute it and/or modify  #
rem #    it under the terms of the GNU General Public License as published by  #
rem #    the Free Software Foundation, either version 3 of the License, or     #
rem #    (at your option) any later version.                                   #
rem #                                                                          #
rem #    This program is distributed in the hope that it will be useful,       #
rem #    but WITHOUT ANY WARRANTY; without even the implied warranty of        #
rem #    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
rem #    GNU General Public License for more details.                          #
rem #                                                                          #
rem #    You should have received a copy of the GNU General Public License     #
rem #    along with this program.  If not, see <http://www.gnu.org/licenses/>. #
rem #                                                                          #
rem ############################################################################


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

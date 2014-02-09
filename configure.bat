@echo off
rem ############################################################################
rem #                                                                          #
rem #    This file is part of Virtual Chess Clock, a chess clock software      #
rem #                                                                          #
rem #    Copyright (C) 2010-2014 Yoann Le Montagner <yo35(at)melix(dot)net>    #
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


rem Help text
goto end_help
:begin_help
	echo Usage: configure.bat [options]
	echo Available options:
	echo   --prefix=PREFIX  install files at PREFIX (default: %ProgramFiles%\virtual-chess-clock)
	echo   --help           print this message
	exit /B
:end_help

rem Default installation directory
set CMAKE_INSTALL_PREFIX="%ProgramFiles%\virtual-chess-clock"

rem Parse the arguments
:loop_parse
	if "%1"=="" goto end_loop_parse
	if %1==--prefix goto consume_prefix
	goto begin_help

	:consume_prefix
		shift
		set CMAKE_INSTALL_PREFIX=%1
		goto end_consume

	:end_consume
		shift
		goto loop_parse

:end_loop_parse

rem Call to CMake
echo This script is only a frontend for cmake. You can also run cmake directly.
cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%CMAKE_INSTALL_PREFIX% -G"MinGW Makefiles"

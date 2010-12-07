#!/bin/sh
################################################################################
#                                                                              #
#    This file is part of Virtual Chess Clock, a chess clock software          #
#                                                                              #
#    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>            #
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


# Fichier temporaire
TMP_OUTPUT_FILE=tmp_output_file

# Extraction du journal subversion
svn2cl --group-by-day

# Supprime les lignes concernant ChangeLog dans ChangLog
sed -e '/* ChangeLog:/d' ChangeLog > $TMP_OUTPUT_FILE
mv $TMP_OUTPUT_FILE ChangeLog

# Modifie les noms des développeurs
sed \
	-e 's/\([0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]\)  yoann$/\1  Yoann Le Montagner <yo35(at)melix(dot)net>/g' \
	-e 's/\([0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]\)  Administrateur$/\1  Yoann Le Montagner <yo35(at)melix(dot)net>/g' \
	-e 's/\([0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]\)  Yoann$/\1  Yoann Le Montagner <yo35(at)melix(dot)net>/g' \
	ChangeLog > $TMP_OUTPUT_FILE
mv $TMP_OUTPUT_FILE ChangeLog

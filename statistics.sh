#!/bin/bash
################################################################################
#                                                                              #
#    This file is part of Virtual Chess Clock, a chess clock software          #
#                                                                              #
#    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>       #
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


# Fichiers sources
SRC_FILES=`ls src/*.{h,cpp,inl} src/winkeyhook/*.c include/*.h`

# Nombre total de lignes
STOC_TOTAL=`cat $SRC_FILES | wc -l`

# Nombre de lignes non-vides
STOC_NON_EMPTY=`cat $SRC_FILES | sed -e '
	/^\s*$/d
' | wc -l`

# Nombre de lignes avec commentaire seul
STOC_COMM_CPP_ALONE=`cat $SRC_FILES | sed -n -e '
	/^\s*\/\/.*$/p
' | wc -l`
STOC_COMM_C_ALONE=`cat $SRC_FILES | sed -n -e '
	/^\s*\/\*.*\*\/\s*$/p
' | wc -l`

# Nombre de lignes dans un bloc de commentaire (/* qqch */ sur plusieurs lignes)
STOC_COMM_BLOC=`cat $SRC_FILES | sed -n -e '
	/\/\*.*\*\//d
	/\/\*/,/\*\// {
		p
	}
' | wc -l`

# Nombre total de lignes de commentaire
STOC_COMM_ALL=$[$STOC_COMM_CPP_ALONE + $STOC_COMM_C_ALONE + $STOC_COMM_BLOC]

# Code pure
STOC_CODE_ONLY=$[$STOC_NON_EMPTY - $STOC_COMM_ALL]

# Affichage
echo -e "Total code lines:\t $STOC_TOTAL"
echo -e "Non-empty lines: \t $STOC_NON_EMPTY\t ($[$STOC_NON_EMPTY*100/$STOC_TOTAL] %)"
echo -e "Comment lines:   \t $STOC_COMM_ALL\t ($[$STOC_COMM_ALL*100/$STOC_TOTAL] %)"
echo -e "Pure code lines: \t $STOC_CODE_ONLY\t ($[$STOC_CODE_ONLY*100/$STOC_TOTAL] %)"

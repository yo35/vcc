#!/bin/bash
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


# Source files
SRC_FILES=$*

# Number of files
STOC_FILES=`echo $SRC_FILES | wc -w`

# Total number of lines
STOC_TOTAL=`cat $SRC_FILES | wc -l`

# Number of non-empty lines
STOC_NON_EMPTY=`cat $SRC_FILES | sed -e '
	/^\s*$/d
' | wc -l`

# Number of C++-style comment lines (such as: `// comment`)
STOC_COMM_CPP_ALONE=`cat $SRC_FILES | sed -n -e '
	/^\s*\/\/.*$/p
' | wc -l`

# Number of C-style 1-line comment lines (such as: `/* comment */`)
STOC_COMM_C_ALONE=`cat $SRC_FILES | sed -n -e '
	/^\s*\/\*.*\*\/\s*$/p
' | wc -l`

# Number of C-style bloc comment lines (such as: `/* comment */` spanning several lines)
STOC_COMM_BLOC=`cat $SRC_FILES | sed -n -e '
	/\/\*.*\*\//d
	/\/\*/,/\*\// {
		p
	}
' | wc -l`

# Total number of comment lines
STOC_COMM_ALL=$[$STOC_COMM_CPP_ALONE + $STOC_COMM_C_ALONE + $STOC_COMM_BLOC]

# Pure code
STOC_CODE_ONLY=$[$STOC_NON_EMPTY - $STOC_COMM_ALL]

# Print the result
echo -e "Number of files: \t $STOC_FILES"
echo -e "Total code lines:\t $STOC_TOTAL"
echo -e "Non-empty lines: \t $STOC_NON_EMPTY\t ($[$STOC_NON_EMPTY*100/$STOC_TOTAL] %)"
echo -e "Comment lines:   \t $STOC_COMM_ALL\t ($[$STOC_COMM_ALL*100/$STOC_TOTAL] %)"
echo -e "Pure code lines: \t $STOC_CODE_ONLY\t ($[$STOC_CODE_ONLY*100/$STOC_TOTAL] %)"

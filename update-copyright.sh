#!/bin/bash
################################################################################
#                                                                              #
#    This file is part of Virtual Chess Clock, a chess clock software          #
#                                                                              #
#    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>        #
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


# Message de copyright
COPYRIGHT_MESSAGE="Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>"

# Vérification copyright
COPYRIGHT_MESSAGE=`echo $COPYRIGHT_MESSAGE | sed -n -e '/^Copyright/p'`
LENGTH_MESSAGE=$[`echo $COPYRIGHT_MESSAGE | wc -c` - 1]
if [ "$LENGTH_MESSAGE" -eq 0 ]; then
	echo "Invalid copyright message"
	exit
elif [ "$LENGTH_MESSAGE" -gt 70 ]; then
	echo "The message is too long"
	exit
fi
COPYRIGHT_MESSAGE=`echo $COPYRIGHT_MESSAGE | sed -e 's/ /<SPACE>/g'`

# Fichier temporaire
TMP_FILE="tmp_file"

# Espaces de fin de ligne
LENGTH_PADDING_C_FILE=$[80 - 8 - $LENGTH_MESSAGE]
LENGTH_PADDING_SH_FILE=$[80 - 6 - $LENGTH_MESSAGE]
LENGTH_PADDING_BAT_FILE=$[80 - 10 - $LENGTH_MESSAGE]

# Fonction générant les chaînes de padding
function make_padder {
	RETVAL=""
	COUNTER=$1
	while [ "$COUNTER" -gt 0 ]
	do
		RETVAL=$RETVAL"<SPACE>"
		COUNTER=$[$COUNTER - 1]
	done
	echo $RETVAL
}
PADDER_PREFIX="<SPACE><SPACE><SPACE><SPACE>"
PADDER_C_FILE=$(make_padder $LENGTH_PADDING_C_FILE)
PADDER_SH_FILE=$(make_padder $LENGTH_PADDING_SH_FILE)
PADDER_BAT_FILE=$(make_padder $LENGTH_PADDING_BAT_FILE)

# MAJ des fichiers
for FILE in $*
do
	echo "Updating "$FILE"..."
	sed -i -e 's/^ \*.*Copyright.*\*/ *'$PADDER_PREFIX$COPYRIGHT_MESSAGE$PADDER_C_FILE'*/g' $FILE
	sed -i -e 's/^#.*Copyright.*#/#'$PADDER_PREFIX$COPYRIGHT_MESSAGE$PADDER_SH_FILE'#/g' $FILE
	sed -i -e 's/^rem #.*Copyright.*#/rem #'$PADDER_PREFIX$COPYRIGHT_MESSAGE$PADDER_BAT_FILE'#/g' $FILE
	sed -i -e '/Copyright/ {
		s/<SPACE>/ /g
	}' $FILE
done

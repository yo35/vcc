#!/bin/sh

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

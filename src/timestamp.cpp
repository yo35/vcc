/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                            *
 ******************************************************************************/


#include "timestamp.h"

// Calcule (t2 - t1), et retourne le résultat en millisecondes
int difftime(const Timestamp &t2, const Timestamp &t1) {
	return
		(t2.tv_sec  - t1.tv_sec ) * 1000 +
		(t2.tv_usec - t1.tv_usec) / 1000;
}

// Retourne la date présente
Timestamp get_timestamp_now() {
	Timestamp res;
	gettimeofday(&res, 0);
	return res;
}

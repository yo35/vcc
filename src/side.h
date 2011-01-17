/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2011  Yoann Le Montagner <yo35(at)melix(dot)net>     *
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


#ifndef SIDE_H
#define SIDE_H

#include "enumerable.h"

// Type de base
struct BaseSide {
	static const int N = 2;
};

// Type énuméré
typedef Enumerable<BaseSide> Side;

// Constantes
extern const Side LEFT ;
extern const Side RIGHT;

// Container
typedef EnumArray<Side, int >  IntSideArray;
typedef EnumArray<Side, bool> BoolSideArray;

// Fonctions algorithmique
Side rev(const Side &side);


#endif

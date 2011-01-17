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


#ifndef OPTIONENUMS_H
#define OPTIONENUMS_H

#include "enumerable.h"

// Demande de confirmation pour la réinitialisation de l'horloge
struct BaseReinitConfirm {
	static const int N = 3;
};
typedef Enumerable<BaseReinitConfirm> ReinitConfirm;

extern const ReinitConfirm RC_ALWAYS       ;
extern const ReinitConfirm RC_IF_NOT_PAUSED;
extern const ReinitConfirm RC_NEVER        ;



// Combinaisons de touches permettant de déclencher la remise à zéro
struct BaseKeyCombination {
	static const int N = 2;
};
typedef Enumerable<BaseKeyCombination> KeyCombination;

extern const KeyCombination DOUBLE_CTRL;
extern const KeyCombination DOUBLE_MAJ ;


#endif

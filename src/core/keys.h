/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2014 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


#ifndef KEYS_H_
#define KEYS_H_

#include "enumutil.h"
#include <cstdint>


/**
 * The physical keys of the keyboard are identified by a scan-code. This is a low-level
 * identification, that is likely to depends on the Operating System.
 */
typedef std::uint32_t ScanCode;


/**
 * Pair of modifier keys that can be pressed to change the function associated to
 * some keys on the keyboard.
 */
enum class ModifierKeys : std::uint8_t
{
	DOUBLE_CTRL ,
	DOUBLE_SHIFT,
	DOUBLE_ALT
};

namespace Enum { template<> struct traits<ModifierKeys> : trait_indexing<3> {}; }


#endif /* KEYS_H_ */

/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


#ifndef SIDE_H_
#define SIDE_H_

#include "enumutil.h"
#include <cstdint>


/**
 * Side of the clock.
 */
enum class Side : std::uint8_t
{
	LEFT ,
	RIGHT
};

namespace Enum { template<> struct traits<Side> : trait_indexing<2> {}; }



/**
 * Return the opposite side of the clock with respect to `s`.
 */
constexpr Side flip(Side s) { return Enum::from_value<Side>(1 - Enum::to_value(s)); }


#endif /* SIDE_H_ */

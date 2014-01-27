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


#include "versionnumber.h"
#include <sstream>


// Full version.
std::string VersionNumber::full_number() const
{
	std::stringstream buffer;
	buffer << *this;
	return buffer.str();
}


// Short version.
std::string VersionNumber::short_number() const
{
	std::stringstream buffer;
	buffer << _major << "." << _minor;
	if(_patch!=0) {
		buffer << "." << _patch;
	}
	return buffer.str();
}


// Comparison operator (equal).
bool VersionNumber::operator==(const VersionNumber &op) const
{
	return _major==op._major && _minor==op._minor && _patch==op._patch;
}


// Comparison operator (less than).
bool VersionNumber::operator<(const VersionNumber &op) const
{
	return
		_major<op._major || (_major==op._major && (
		_minor<op._minor || (_minor==op._minor && (
		_patch<op._patch
		))));
}


// Output stream operator.
std::ostream &operator<<(std::ostream &stream, const VersionNumber &version_number)
{
	return stream << version_number._major << "." << version_number._minor << "." << version_number._patch;
}

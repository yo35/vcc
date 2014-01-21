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


#ifndef VERSIONNUMBER_H_
#define VERSIONNUMBER_H_

#include <string>
#include <ostream>


/**
 * Version number.
 */
class VersionNumber
{
public:

	/**
	 * Default constructor (version 0.0.0).
	 */
	VersionNumber() : _major(0), _minor(0), _patch(0) {}

	/**
	 * Constructor.
	 */
	VersionNumber(int major, int minor, int patch=0)  : _major(major), _minor(minor), _patch(patch) {}

	/**
	 * Major version number.
	 */
	int major() const { return _major; }

	/**
	 * Minor version number.
	 */
	int minor() const { return _minor; }

	/**
	 * Patch number
	 */
	int patch() const { return _patch; }

	/**
	 * Set the major version number.
	 */
	void set_major(int value) { _major = value; }

	/**
	 * Set the minor version number.
	 */
	void set_minor(int value) { _minor = value; }

	/**
	 * Set the patch number.
	 */
	void set_patch(int value) { _patch = value; }

	/**
	 * Display the version number as MAJOR.MINOR.PATCH.
	 */
	std::string full_number() const;

	/**
	 * Display the version without the patch number if it is 0.
	 */
	std::string short_number() const;

	/**
	 * @name Comparison operators.
	 * @{
	 */
	bool operator==(const VersionNumber &op) const;
	bool operator!=(const VersionNumber &op) const { return !operator==(op); }
	bool operator<=(const VersionNumber &op) const { return  operator==(op) || operator<(op); }
	bool operator< (const VersionNumber &op) const;
	bool operator>=(const VersionNumber &op) const { return !operator< (op); }
	bool operator> (const VersionNumber &op) const { return !operator<=(op); }
	/**@} */

	/**
	 * Output stream operator for version numbers.
	 */
	friend std::ostream &operator<<(std::ostream &stream, const VersionNumber &version_number);

private:

	// Private members
	int _major;
	int _minor;
	int _patch;
};


#endif /* VERSIONNUMBER_H_ */

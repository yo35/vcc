/******************************************************************************
 *                                                                            *
 *    Heads Up Tree Inspector                                                 *
 *                                                                            *
 *    Copyright (C) 2011-2012 Yoann Le Montagner <yo35@melix.net>             *
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

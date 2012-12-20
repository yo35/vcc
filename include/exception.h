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


#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <string>
#include <boost/exception/exception.hpp>
#include <boost/exception/info.hpp>

/**
 * Piece of information attached to an exception
 */
#define INFO_EXCEPTION(name, type) struct _ ## name {}; typedef boost::error_info<_ ## name, type> name

/**
 * Throw an exception
 */
#define THROW_EXCEPTION(err) BOOST_THROW_EXCEPTION(err)

/**
 * All the exception defined in the program inherits from this base class
 */
class BaseException : virtual public std::exception, virtual public boost::exception {};

/**
 * Runtime exception
 */
class RuntimeException : virtual public BaseException
{
public:

	/**
	 * Destructor
	 */
	virtual ~RuntimeException() throw () {}

	/**
	 * Default constructor, no error message
	 */
	RuntimeException() throw () : BaseException() {}

	/**
	 * Constructor specifying an error message
	 */
	explicit RuntimeException(std::string what) throw () : BaseException(), m_what(std::move(what)) {}

	/**
	 * Return the error message
	 */
	virtual const char *what() const throw () { return m_what.c_str(); }

private:

	// Private data
	std::string m_what;
};

#endif

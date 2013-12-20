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


#ifndef TRANSLATION_H_
#define TRANSLATION_H_

#include <QObject>
#include <string>
#include <utility>
#include <ostream>


/**
 * Mark a string `s` for translation.
 */
#define _(s) QStringWrapper(QObject::tr(s))


/**
 * This class is used in the macro `_()` to wrap the QString object returned by
 * the translation function `QObject::tr()`, to provide automatic cast into
 * the common string classes while avoiding as much as possible useless conversions.
 *
 * `QStringWrapper` is not supposed to be used in a different context.
 */
class QStringWrapper
{
public:

	/**
	 * @name Neither copy nor move is allowed.
	 * @{
	 */
	QStringWrapper(const QStringWrapper &op) = delete;
	QStringWrapper(QStringWrapper &&op) = delete;
	QStringWrapper &operator=(const QStringWrapper &op) = delete;
	QStringWrapper &operator=(QStringWrapper &&op) = delete;
	/**@} */

	/**
	 * Constructor.
	 */
	explicit QStringWrapper(QString &&data) : _data(std::move(data)) {}

	/**
	 * Native conversion.
	 */
	operator QString() { return std::move(_data); }

	/**
	 * Conversion into a standard C++ string.
	 */
	operator std::string() const { return _data.toStdString(); }

	/**
	 * Conversion into a null-terminated char array.
	 */
	explicit operator const char *() const { return _data.toLocal8Bit().constData(); }

	/**
	 * Output stream operator.
	 */
	friend std::ostream &operator<<(std::ostream &stream, const QStringWrapper &wrapper)
	{
		return stream << static_cast<const char *>(wrapper);
	}

private:

	// Private members
	QString _data;
};

#endif /* TRANSLATION_H_ */

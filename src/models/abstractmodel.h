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


#ifndef ABSTRACTMODEL_H_
#define ABSTRACTMODEL_H_

#include <set>
#include <core/property.h>


/**
 * Base class for models.
 */
class AbstractModel
{
public:

	/**
	 * Destructor.
	 */
	virtual ~AbstractModel() {}

	/**
	 * @name Neither copy nor move is allowed.
	 * @{
	 */
	AbstractModel(const AbstractModel &op) = delete;
	AbstractModel(AbstractModel &&op) = delete;
	AbstractModel &operator=(const AbstractModel &op) = delete;
	AbstractModel &operator=(AbstractModel &&op) = delete;
	/**@} */

	/**
	 * Save the model.
	 */
	void save();

protected:

	/**
	 * Constructor.
	 */
	AbstractModel() : _shunt_saved(false) {}

	/**
	 * Register the given constant property.
	 */
	void register_property(AbstractReadOnlyProperty &property);

	/**
	 * Register the given writable property.
	 */
	void register_property(AbstractReadWriteProperty &property);

	/**
	 * Called at the end of the `save()` method. Nothing done by default.
	 */
	virtual void finalize_save() {}

private:

	// Private methods
	void on_property_saved();

	// Private members
	bool _shunt_saved;
	std::set<AbstractReadWriteProperty *> _read_write_properties;
};

#endif /* ABSTRACTMODEL_H_ */

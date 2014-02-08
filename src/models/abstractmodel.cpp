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


#include "abstractmodel.h"


// Register the given constant property.
void AbstractModel::register_property(AbstractReadOnlyProperty &)
{
	// Nothing to do.
}


// Register the given writable property.
void AbstractModel::register_property(AbstractReadWriteProperty &property)
{
	property.connect_saved(boost::bind(&AbstractModel::on_property_saved, this));
	_read_write_properties.insert(&property);
}


// Save the model.
void AbstractModel::save()
{
	bool need_finalize_save = false;
	_shunt_saved = true;
	for(auto it : _read_write_properties) {
		need_finalize_save = it->save() || need_finalize_save;
	}
	if(need_finalize_save) {
		finalize_save();
	}
	_shunt_saved = false;
}


// Handler called when one of the registered property is saved.
void AbstractModel::on_property_saved()
{
	if(_shunt_saved) {
		return;
	}
	finalize_save();
}

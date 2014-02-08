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


#ifndef MODELSHORTCUTMAP_H_
#define MODELSHORTCUTMAP_H_

#include "abstractmodel.h"
#include <core/singleton.h>
#include <core/shortcutmap.h>


/**
 * Model holding the preferences of the user relative to the key shortcuts.
 */
class ModelShortcutMap : public AbstractModel, public Singleton<ModelShortcutMap>
{
	friend class Singleton<ModelShortcutMap>;

public:

	/**
	 * Path to the default shortcut map file.
	 */
	ReadOnlyProperty<std::string> default_shortcut_map_file;

	/**
	 * Path to the user-specific shortcut map file.
	 */
	ReadOnlyProperty<std::string> custom_shortcut_map_file;

	/**
	 * Shortcut map in use.
	 */
	ReadWriteProperty<ShortcutMap> shortcut_map;

private:

	// Constructor
	ModelShortcutMap();

	// Loaders (read-only properties)
	void load_default_shortcut_map_file(std::string &target);
	void load_custom_shortcut_map_file (std::string &target);

	// Shortcut map management.
	void load_shortcut_map(ShortcutMap &target);
	void save_shortcut_map(const ShortcutMap &value);
};

#endif /* MODELSHORTCUTMAP_H_ */

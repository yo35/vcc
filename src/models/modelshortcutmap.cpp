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


#include "modelshortcutmap.h"
#include "modelpaths.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>


// Constructor
ModelShortcutMap::ModelShortcutMap() :
	default_shortcut_map_file(std::bind(&ModelShortcutMap::load_default_shortcut_map_file, this, std::placeholders::_1)),
	custom_shortcut_map_file (std::bind(&ModelShortcutMap::load_custom_shortcut_map_file , this, std::placeholders::_1)),
	shortcut_map(
		std::bind(&ModelShortcutMap::load_shortcut_map, this, std::placeholders::_1),
		std::bind(&ModelShortcutMap::save_shortcut_map, this, std::placeholders::_1)
	)
{
	register_property(default_shortcut_map_file);
	register_property(custom_shortcut_map_file );
	register_property(shortcut_map             );
}


// Load the shortcut map file.
void ModelShortcutMap::load_shortcut_map(ShortcutMap &target)
{
	try
	{
		// Try to load the user-defined shortcut map file if it exists.
		if(boost::filesystem::exists(custom_shortcut_map_file())) {
			target.load(custom_shortcut_map_file());
		}

		// Otherwise, try to load the default shortcut map file.
		else {
			target.load(default_shortcut_map_file());
		}
	}

	// The keyboard index file must be readable.
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while reading the shortcut map file.");
	}
}


// Save the shortcut map file.
void ModelShortcutMap::save_shortcut_map(const ShortcutMap &value)
{
	// Create the parent folder if necessary.
	ModelPaths::instance().ensure_config_path_exists();

	// Write the file.
	try {
		value.save(custom_shortcut_map_file());
	}
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while writing the shortcut map file.");
	}
}



// *****************************************************************************
// Loaders (read-only properties)
// *****************************************************************************


void ModelShortcutMap::load_default_shortcut_map_file(std::string &target)
{
	target = ModelPaths::instance().share_path() + "/default-shortcut-map.xml";
}


void ModelShortcutMap::load_custom_shortcut_map_file(std::string &target)
{
	target = ModelPaths::instance().config_path() + "/shortcut-map.xml";
}

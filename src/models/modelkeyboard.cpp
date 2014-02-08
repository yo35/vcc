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


#include "modelkeyboard.h"
#include "modelpaths.h"
#include "modelappinfo.h"
#include <stdexcept>
#include <boost/property_tree/xml_parser.hpp>


// Constructor
ModelKeyboard::ModelKeyboard() :
	index_file(std::bind(&ModelKeyboard::load_index_file, this, std::placeholders::_1)),
	default_id(std::bind(&ModelKeyboard::load_default_id, this, std::placeholders::_1)),
	ids       (std::bind(&ModelKeyboard::load_ids       , this, std::placeholders::_1))
{
	register_property(index_file);
	register_property(default_id);
	register_property(ids       );
}


// Return the keyboard map corresponding to the given ID.
const KeyboardMap &ModelKeyboard::keyboard_map(const std::string &id)
{
	ensure_id_exists(id);
	auto it = _keyboard_maps.find(id);
	if(it==_keyboard_maps.end()) {
		try {
			return _keyboard_maps[id].load(ModelPaths::instance().share_path() + "/keyboard-maps/" + id + ".kbm");
		}
		catch(boost::property_tree::ptree_error &) {
			throw std::runtime_error("An error has occurred while reading a keyboard map file.");
		}
	}
	return it->second;
}


// Throw an exception if the given keyboard ID exists and is registered in the keyboard index file.
void ModelKeyboard::ensure_id_exists(const std::string &id)
{
	if(ids().count(id)==0) {
		throw std::invalid_argument("Invalid keyboard ID.");
	}
}



// *****************************************************************************
// Loaders
// *****************************************************************************


void ModelKeyboard::load_index_file(std::string &target)
{
	target = ModelPaths::instance().share_path() + "/keyboards.xml";
}


void ModelKeyboard::load_default_id(std::string &target)
{
	ids.load(); // <- Ensure that the keyboard index file is loaded.
	auto it = _locale_to_id.find(ModelAppInfo::instance().locale());
	target = it==_locale_to_id.end() ? _wildcard_id : it->second;
}


void ModelKeyboard::load_ids(std::set<std::string> &target)
{
	try
	{
		// Read the keyboard index file
		boost::property_tree::ptree keyboard_index;
		boost::property_tree::read_xml(index_file(), keyboard_index, boost::property_tree::xml_parser::trim_whitespace);

		// Iterates over the list of keyboards
		const boost::property_tree::ptree &keyboards(keyboard_index.get_child("keyboards"));
		for(const auto &it : keyboards) {
			if(it.first!="keyboard") {
				continue;
			}
			load_keyboard(target, it.second);
		}
	}

	// The keyboard index file must be readable.
	catch(boost::property_tree::xml_parser_error &) {
		throw std::runtime_error("An error has occurred while reading the keyboard index file.");
	}
}


void ModelKeyboard::load_keyboard(std::set<std::string> &target, const boost::property_tree::ptree &keyboard)
{
	// Read the ID/name/icon data associated to the given keyboard.
	std::string id   = keyboard.get<std::string>("id"  );
	std::string icon = keyboard.get<std::string>("icon");
	_names[id] = keyboard.get<std::string>("name");
	_icons[id] = icon.empty() ? QIcon() : QIcon(QString::fromStdString(ModelPaths::instance().share_path() + "/flags/" + icon));

	// List the locales for which the keyboard will be considered as the default one.
	for(const auto &it : keyboard.get_child("locales")) {
		if(it.first!="locale") {
			continue;
		}
		std::string locale = it.second.get_value<std::string>();
		if(locale=="*") {
			_wildcard_id = id;
		}
		else {
			_locale_to_id[locale] = id;
		}
	}

	// Register the keyboard in the list of available keyboards.
	target.insert(std::move(id));
}

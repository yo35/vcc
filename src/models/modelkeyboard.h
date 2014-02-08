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


#ifndef MODELKEYBOARD_H_
#define MODELKEYBOARD_H_

#include "abstractmodel.h"
#include <core/singleton.h>
#include <core/keyboardmap.h>
#include <map>
#include <set>
#include <string>
#include <QIcon>
#include <boost/property_tree/ptree.hpp>


/**
 * Model providing keyboard-map related information.
 */
class ModelKeyboard : public AbstractModel, public Singleton<ModelKeyboard>
{
	friend class Singleton<ModelKeyboard>;

public:

	/**
	 * Path to the file that indexes all the available keyboard maps.
	 */
	ReadOnlyProperty<std::string> index_file;

	/**
	 * ID of the default keyboard (determined according to the current locale).
	 */
	ReadOnlyProperty<std::string> default_id;

	/**
	 * IDs of all the available keyboard maps.
	 */
	ReadOnlyProperty<std::set<std::string>> ids;

	/**
	 * Return the name of the keyboard map corresponding to the given ID.
	 */
	const std::string &name(const std::string &id) { ensure_id_exists(id); return _names.find(id)->second; }

	/**
	 * Return the icon of the keyboard map corresponding to the given ID.
	 */
	QIcon icon(const std::string &id) { ensure_id_exists(id); return _icons.find(id)->second; }

	/**
	 * Return the keyboard map corresponding to the given ID.
	 */
	const KeyboardMap &keyboard_map(const std::string &id);

private:

	// Constructor
	ModelKeyboard();

	// Loaders
	void load_index_file(std::string           &target);
	void load_default_id(std::string           &target);
	void load_ids       (std::set<std::string> &target);
	void load_keyboard  (std::set<std::string> &target, const boost::property_tree::ptree &keyboard);

	// Private methods
	void ensure_id_exists(const std::string &id);

	// Private members
	std::map<std::string, std::string> _names        ;
	std::map<std::string, QIcon      > _icons        ;
	std::map<std::string, std::string> _locale_to_id ;
	std::string                        _wildcard_id  ;
	std::map<std::string, KeyboardMap> _keyboard_maps;
};

#endif /* MODELKEYBOARD_H_ */

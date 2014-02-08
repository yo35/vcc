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


#ifndef MODELPATHS_H_
#define MODELPATHS_H_

#include "abstractmodel.h"
#include <core/singleton.h>
#include <string>


/**
 * Model holding the paths to the folders used by the application.
 */
class ModelPaths : public AbstractModel, public Singleton<ModelPaths>
{
	friend class Singleton<ModelPaths>;

public:

	/**
	 * Directory holding data shared by all the users (read-only directory).
	 */
	ReadOnlyProperty<std::string> share_path;

	/**
	 * Directory holding data that is user-specific (read-write directory).
	 */
	ReadOnlyProperty<std::string> config_path;

	/**
	 * Directory holding the translation files (read-only directory).
	 */
	ReadOnlyProperty<std::string> translation_path;

	/**
	 * Ensure that the config path exists.
	 */
	void ensure_config_path_exists();

private:

	// Constructor.
	ModelPaths();

	// Loaders.
	void load_share_path      (std::string &target);
	void load_config_path     (std::string &target);
	void load_translation_path(std::string &target);
};

#endif /* MODELPATHS_H_ */

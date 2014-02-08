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


#ifndef MODELAPPINFO_H_
#define MODELAPPINFO_H_

#include "abstractmodel.h"
#include <core/singleton.h>
#include <core/versionnumber.h>
#include <string>
#include <map>
#include <QIcon>


/**
 * Model holding some general application-related information.
 */
class ModelAppInfo : public AbstractModel, public Singleton<ModelAppInfo>
{
	friend class Singleton<ModelAppInfo>;

public:

	/**
	 * Application short-name.
	 */
	ReadOnlyProperty<std::string> short_name;

	/**
	 * Application name.
	 */
	ReadOnlyProperty<std::string> name;

	/**
	 * Application full-name.
	 */
	ReadOnlyProperty<std::string> full_name;

	/**
	 * Application version number.
	 */
	ReadOnlyProperty<VersionNumber> version;

	/**
	 * Current locale.
	 */
	ReadOnlyProperty<std::string> locale;

	/**
	 * Application icon.
	 */
	ReadOnlyProperty<QIcon> icon;

	/**
	 * Tool-bar icon.
	 */
	const QIcon &toolbar_icon(const std::string &name);

private:

	// Constructor.
	ModelAppInfo();

	// Loaders.
	void load_short_name(std::string   &target);
	void load_name      (std::string   &target);
	void load_full_name (std::string   &target);
	void load_version   (VersionNumber &target);
	void load_locale    (std::string   &target);
	void load_icon      (QIcon         &target);

	// Private members
	std::map<std::string, QIcon> _toolbar_icons;
};

#endif /* MODELAPPINFO_H_ */

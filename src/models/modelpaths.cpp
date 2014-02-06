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


#include "modelpaths.h"
#include <config.h>
#include <QApplication>
#include <QStandardPaths>
#include <boost/filesystem.hpp>


// Ensure that the config path exists.
void ModelPaths::ensure_config_path_exists()
{
	boost::filesystem::create_directories(config_path());
}


// Constructor.
ModelPaths::ModelPaths() :
	share_path      (std::bind(&ModelPaths::load_share_path      , this, std::placeholders::_1)),
	config_path     (std::bind(&ModelPaths::load_config_path     , this, std::placeholders::_1)),
	translation_path(std::bind(&ModelPaths::load_translation_path, this, std::placeholders::_1))
{
	register_property(share_path      );
	register_property(config_path     );
	register_property(translation_path);
}



// *****************************************************************************
// Loaders
// *****************************************************************************


void ModelPaths::load_share_path(std::string &target)
{
	#ifdef VCC_DEVELOPMENT_SETTINGS
		target = SHARE_PATH;
	#else
		#if defined(OS_IS_WINDOWS)
			#define SHARE_PATH_SUFFIX "/share"
		#elif defined(OS_IS_UNIX)
			#define SHARE_PATH_SUFFIX "/../share/" APP_NAME
		#endif
		target = QCoreApplication::applicationDirPath().toStdString() + SHARE_PATH_SUFFIX;
		#undef SHARE_PATH_SUFFIX
	#endif
}


void ModelPaths::load_config_path(std::string &target)
{
	#ifdef VCC_DEVELOPMENT_SETTINGS
		target = CONFIG_PATH;
	#else
		target = QStandardPaths::writableLocation(QStandardPaths::DataLocation).toStdString();
	#endif
}


void ModelPaths::load_translation_path(std::string &target)
{
	#ifdef VCC_DEVELOPMENT_SETTINGS
		target = TRANSLATION_PATH;
	#else
		target = share_path() + "/translation";
	#endif
}

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


#include "modelappinfo.h"
#include "modelpaths.h"
#include <config.h>
#include <QApplication>


// Constructor.
ModelAppInfo::ModelAppInfo() :
	short_name(std::bind(&ModelAppInfo::load_short_name, this, std::placeholders::_1)),
	name      (std::bind(&ModelAppInfo::load_name      , this, std::placeholders::_1)),
	full_name (std::bind(&ModelAppInfo::load_full_name , this, std::placeholders::_1)),
	version   (std::bind(&ModelAppInfo::load_version   , this, std::placeholders::_1)),
	locale    (std::bind(&ModelAppInfo::load_locale    , this, std::placeholders::_1)),
	icon      (std::bind(&ModelAppInfo::load_icon      , this, std::placeholders::_1))
{
	register_property(short_name);
	register_property(name      );
	register_property(full_name );
	register_property(version   );
	register_property(locale    );
	register_property(icon      );
}



// *****************************************************************************
// Loaders
// *****************************************************************************


void ModelAppInfo::load_short_name(std::string &target)
{
	target = APP_SHORT_NAME;
}


void ModelAppInfo::load_name(std::string &target)
{
	target = APP_NAME;
}

void ModelAppInfo::load_full_name(std::string &target)
{
	target = APP_FULL_NAME;
}

void ModelAppInfo::load_version(VersionNumber &target)
{
	target = VersionNumber(APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);
}

void ModelAppInfo::load_locale(std::string &target)
{
	target = QApplication::inputMethod()->locale().name().toStdString();
}


void ModelAppInfo::load_icon(QIcon &target)
{
	ModelPaths &model(ModelPaths::instance());
	target.addFile(QString::fromStdString(model.share_path() + "/vcc-small.png"));
	target.addFile(QString::fromStdString(model.share_path() + "/vcc.png"      ));
	target.addFile(QString::fromStdString(model.share_path() + "/vcc-large.png"));
}

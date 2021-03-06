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


#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "mainwindow.h"
#include <models/modelpaths.h>
#include <models/modelappinfo.h>


int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setApplicationName(QString::fromStdString(ModelAppInfo::instance().name()));

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	QTranslator customTranslator;
	customTranslator.load(QLocale::system(), "", "", QString::fromStdString(ModelPaths::instance().translation_path()));
	app.installTranslator(&customTranslator);

	MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}

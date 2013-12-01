/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010-2012 Yoann Le Montagner <yo35(at)melix(dot)net>      *
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


#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include "options.h"
#include "bitimer.h"
class DialWidget;


/**
 * Main window of the application.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	MainWindow();


protected:

	/**
	 * Close event handler.
	 */
	void closeEvent(QCloseEvent *event) override;

	/**
	 * Key-press event handler.
	 */
	void keyPressEvent(QKeyEvent *event) override;


private:

	// Private functions
	void onResetClicked();
	void onPauseClicked();
	void onTCtrlClicked();
	void onHelpClicked ();
	void onAboutClicked();
	void loadPersistentParameters();
	QIcon fetchIcon(const std::string &name, bool fromTheme=true);

	// Widgets
	Enum::array<Side, DialWidget *> _dial;
	QStatusBar *_statusBar;

	// Private members
	BiTimer           _core             ;
	ResetConfirmation _resetConfirmation;
};

#endif /* MAINWINDOW_H_ */

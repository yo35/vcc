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


#include "mainwindow.h"
#include "params.h"
#include "timecontroldialog.h"
#include <translation.h>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>

#include <iostream>


// Constructor.
MainWindow::MainWindow()
{
	setWindowTitle(QString::fromStdString(Params::get().app_full_name()));
	setWindowIcon(fetchIcon("logo", false));

	// Central widget
	QWidget     *mainWidget = new QWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainWidget->setLayout(mainLayout);
	setCentralWidget(mainWidget);

	// Toolbar
	QToolBar *toolBar = addToolBar(_("Main tool-bar"));
	toolBar->setFloatable(false);
	//toolBar->setMovable(false);
	QAction *actReset = toolBar->addAction(fetchIcon("reset", false), _("Reset"         ));
	QAction *actPause = toolBar->addAction(fetchIcon("pause", false), _("Pause"         ));
	toolBar->addSeparator();
	QAction *actTCtrl = toolBar->addAction(fetchIcon("tctrl", false), _("Time control"  ));
	QAction *actNames = toolBar->addAction(fetchIcon("names", false), _("Players' names"));
	toolBar->addSeparator();
	QAction *actPrefs = toolBar->addAction(fetchIcon("preferences-desktop"), _("Preferences"));
	QAction *actHelp  = toolBar->addAction(fetchIcon("help-contents"      ), _("Help"       ));
	QAction *actAbout = toolBar->addAction(fetchIcon("help-about"         ), _("About"      ));
	actReset->setToolTip(_("Reset the clock"                          ));
	actPause->setToolTip(_("Pause the clock"                          ));
	actTCtrl->setToolTip(_("Change the current time control"          ));
	actNames->setToolTip(_("Edit the names of the players"            ));
	actPrefs->setToolTip(_("Set the configuration and the preferences"));
	actHelp ->setToolTip(_("Show a short help message"                ));
	actAbout->setToolTip(_("Information about credits and license"    ));
	connect(actReset, &QAction::triggered, this, &MainWindow::onResetClicked);
	connect(actPause, &QAction::triggered, this, &MainWindow::onPauseClicked);
	connect(actTCtrl, &QAction::triggered, this, &MainWindow::onTCtrlClicked);

	// Create the widgets
	QHBoxLayout *dialLayout = new QHBoxLayout;
	dialLayout->setSpacing(0);
	for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it) {
		_dial[*it] = new DialWidget(this);
		_dial[*it]->bindTimer(_core, *it);
		dialLayout->addWidget(_dial[*it], 1);
	}
	mainLayout->addLayout(dialLayout, 1);

	// Status bar
	_statusBar = statusBar();

	_core.set_time_control(Params::get().time_control());
	_statusBar->showMessage(QString::fromStdString(_core.time_control().description()));
	loadPersistentParameters();
}


// Close event handler.
void MainWindow::closeEvent(QCloseEvent *)
{
	Params::force_save();
}


// Key-press event handler.
void MainWindow::keyPressEvent(QKeyEvent *event)
{
	std::cout << "Key press: " << event->key() << std::endl;
	int keyCode = event->key();
	if(keyCode==65) // Key A
		_core.start_timer(Side::LEFT);
	else if(keyCode==66) // Key B
		_core.start_timer(Side::RIGHT);
	else if(keyCode==67) // Key C
		_core.stop_timer();
}


// Reset button handler.
void MainWindow::onResetClicked()
{
	if(_resetConfirmation==ResetConfirmation::ALWAYS ||
		(_resetConfirmation==ResetConfirmation::IF_ACTIVE && _core.is_active()))
	{
		auto response = QMessageBox::question(this, _("Stop this game?"), _("Do you really want to start a new game?"));
		if(response!=QMessageBox::Yes) {
			return;
		}
	}
	_core.reset_timers();
}


// Pause button handler.
void MainWindow::onPauseClicked()
{
	_core.stop_timer();
}


// Time control button handler.
void MainWindow::onTCtrlClicked()
{
	TimeControlDialog dialog(this);
	dialog.setTimeControl(_core.time_control());
	if(dialog.exec()!=QDialog::Accepted) {
		return;
	}
	_core.set_time_control(dialog.timeControl());
	_statusBar->showMessage(QString::fromStdString(_core.time_control().description()));
	Params::get().set_time_control(_core.time_control());
}


// Load the persistent parameters saved in the singleton Param object.
void MainWindow::loadPersistentParameters()
{
	// Reset confirmation option
	_resetConfirmation = Params::get().reset_confirmation();

	// Status bar
	_statusBar->setVisible(Params::get().show_status_bar());
}


// Load an icon.
QIcon MainWindow::fetchIcon(const std::string &name, bool fromTheme)
{
	if(fromTheme) {
		return QIcon::fromTheme(QString::fromStdString(name));
	}
	else {
		return QIcon(QString::fromStdString(Params::get().share_path() + "/" + name + ".png"));
	}
}

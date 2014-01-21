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
#include "bitimerwidget.h"
#include "keyboardhandler.h"
#include "timecontroldialog.h"
#include "namedialog.h"
#include "preferencedialog.h"
#include "debugdialog.h"
#include <translation.h>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QEvent>

#include <iostream>


// Constructor.
MainWindow::MainWindow() : _debugDialog(nullptr)
{
	setWindowTitle(QString::fromStdString(Params::get().app_full_name()));
	setWindowIcon(fetchIcon("logo", false));

	// Low-level keyboard handler
	_keyboardHandler = new KeyboardHandler(this);
	connect(_keyboardHandler, &KeyboardHandler::keyPressed, this, &MainWindow::onKeyPressed);

	// Central widget
	QWidget     *mainWidget = new QWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainWidget->setLayout(mainLayout);
	setCentralWidget(mainWidget);

	// Toolbar
	QToolBar *toolBar = addToolBar(_("Main tool-bar"));
	toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
	toolBar->setFloatable(false);
	toolBar->setMovable(false);
	QAction *actReset = toolBar->addAction(fetchIcon("reset", false), _("Reset"         ));
	QAction *actPause = toolBar->addAction(fetchIcon("pause", false), _("Pause"         ));
	QAction *actFlip  = toolBar->addAction(fetchIcon("flip" , false), _("Switch"        ));
	toolBar->addSeparator();
	QAction *actFlScr = toolBar->addAction(fetchIcon("flscr", false), _("Full screen"   ));
	toolBar->addSeparator();
	QAction *actTCtrl = toolBar->addAction(fetchIcon("tctrl", false), _("Time control"  ));
	QAction *actNames = toolBar->addAction(fetchIcon("names", false), _("Players' names"));
	toolBar->addSeparator();
	QAction *actPrefs = toolBar->addAction(fetchIcon("preferences-desktop"   ), _("Preferences"));
	QAction *actHelp  = toolBar->addAction(fetchIcon("help-contents"         ), _("Help"       ));
	QAction *actDebug = toolBar->addAction(fetchIcon("applications-utilities"), _("Debug"      ));
	QAction *actAbout = toolBar->addAction(fetchIcon("help-about"            ), _("About"      ));
	actReset->setToolTip(_("Reset the clock"                                ));
	actPause->setToolTip(_("Pause the clock"                                ));
	actFlip ->setToolTip(_("Switch the players and the time control options"));
	actFlScr->setToolTip(_("Full screen on/off"                             ));
	actTCtrl->setToolTip(_("Change the current time control"                ));
	actNames->setToolTip(_("Edit the names of the players"                  ));
	actPrefs->setToolTip(_("Set the configuration and the preferences"      ));
	actHelp ->setToolTip(_("Show a short help message"                      ));
	actDebug->setToolTip(_("Debug information"                              ));
	actAbout->setToolTip(_("Information about credits and license"          ));
	connect(actReset, &QAction::triggered, this, &MainWindow::onResetClicked);
	connect(actPause, &QAction::triggered, this, &MainWindow::onPauseClicked);
	connect(actTCtrl, &QAction::triggered, this, &MainWindow::onTCtrlClicked);
	connect(actFlScr, &QAction::triggered, this, &MainWindow::onFlScrClicked);
	connect(actNames, &QAction::triggered, this, &MainWindow::onNamesClicked);
	connect(actPrefs, &QAction::triggered, this, &MainWindow::onPrefsClicked);
	connect(actHelp , &QAction::triggered, this, &MainWindow::onHelpClicked );
	connect(actDebug, &QAction::triggered, this, &MainWindow::onDebugClicked);
	connect(actAbout, &QAction::triggered, this, &MainWindow::onAboutClicked);
	actFlScr->setCheckable(true);

	// Bi-timer widget
	_biTimerWidget = new BiTimerWidget(this);
	_biTimerWidget->bindTimer(_biTimer);
	mainLayout->addWidget(_biTimerWidget, 1);

	// Status bar
	_statusBar = statusBar();

	// Load the time control
	_biTimer.set_time_control(Params::get().time_control());
	_statusBar->showMessage(QString::fromStdString(_biTimer.time_control().description()));

	// Load the players' names
	_biTimerWidget->setLabel(Side::LEFT , QString::fromStdString(Params::get().player_name(Side::LEFT )));
	_biTimerWidget->setLabel(Side::RIGHT, QString::fromStdString(Params::get().player_name(Side::RIGHT)));
	_biTimerWidget->setShowLabels(Params::get().show_names());

	// Load the other persistent parameters
	loadPersistentParameters();
}


// Close event handler.
void MainWindow::closeEvent(QCloseEvent *)
{
	Params::force_save();
}


// Window state-change handler.
void MainWindow::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::ActivationChange) {
		_keyboardHandler->setEnabled(isActiveWindow());
	}
	QMainWindow::changeEvent(event);
}


// Key-press event handler.
void MainWindow::onKeyPressed(ScanCode scanCode)
{
	// Retrieve the shortcut that is triggered by the given scan-code, if any.
	bool modifierKeysActivated =
		_keyboardHandler->isDown(_shortcutManager.modifier_key(Side::LEFT )) &&
		_keyboardHandler->isDown(_shortcutManager.modifier_key(Side::RIGHT));
	int shortcut = _shortcutManager.shortcut(scanCode, modifierKeysActivated);

	std::cout << "Key press, key=" << scanCode << " shortcut=" << shortcut << std::endl; ///TODO: remove

	// Execute the requested actions.
	switch(shortcut)
	{
		case 1: _biTimer.start_timer(Side::RIGHT); break;
		case 2: _biTimer.start_timer(Side::LEFT ); break;
		case 3: _biTimer.stop_timer  (); break;
		case 4: _biTimer.reset_timers(); break;
		case 5: break; ///TODO: implement switch
		default: break;
	}
}


// Reset button handler.
void MainWindow::onResetClicked()
{
	if(_resetConfirmation==ResetConfirmation::ALWAYS ||
		(_resetConfirmation==ResetConfirmation::IF_ACTIVE && _biTimer.is_active()))
	{
		auto response = QMessageBox::question(this, _("Stop this game?"), _("Do you really want to start a new game?"));
		if(response!=QMessageBox::Yes) {
			return;
		}
	}
	_biTimer.reset_timers();
}


// Pause button handler.
void MainWindow::onPauseClicked()
{
	_biTimer.stop_timer();
}


// Full-screen button handler.
void MainWindow::onFlScrClicked()
{
	/*// If a previous "normal" state exist, leave the full-screen mode.
	if(_previousState) {
		setWindowState(*_previousState);
		_previousState.reset();
	}

	// Otherwise, save the current "normal" state, and enter the full-screen mode.
	else {
		_previousState = windowState();
		setWindowState(Qt::WindowFullScreen);
	}*/
	///TODO: implement full-screen mode
}


// Time control button handler.
void MainWindow::onTCtrlClicked()
{
	TimeControlDialog dialog(this);
	dialog.setTimeControl(_biTimer.time_control());
	if(dialog.exec()!=QDialog::Accepted) {
		return;
	}
	_biTimer.set_time_control(dialog.timeControl());
	_statusBar->showMessage(QString::fromStdString(_biTimer.time_control().description()));
	Params::get().set_time_control(_biTimer.time_control());
}


// Players' names button handler.
void MainWindow::onNamesClicked()
{
	NameDialog dialog(this);
	dialog.setName(Side::LEFT , _biTimerWidget->label(Side::LEFT ));
	dialog.setName(Side::RIGHT, _biTimerWidget->label(Side::RIGHT));
	dialog.setDisplayNames(_biTimerWidget->showLabels());
	if(dialog.exec()!=QDialog::Accepted) {
		return;
	}
	_biTimerWidget->setLabel(Side::LEFT , dialog.name(Side::LEFT ));
	_biTimerWidget->setLabel(Side::RIGHT, dialog.name(Side::RIGHT));
	_biTimerWidget->setShowLabels(dialog.displayNames());
	Params::get().set_player_name(Side::LEFT , _biTimerWidget->label(Side::LEFT ).toStdString());
	Params::get().set_player_name(Side::RIGHT, _biTimerWidget->label(Side::RIGHT).toStdString());
	Params::get().set_show_names(_biTimerWidget->showLabels());
}


// Preference button handler.
void MainWindow::onPrefsClicked()
{
	PreferenceDialog dialog(this);
	dialog.loadParameters();
	if(dialog.exec()!=QDialog::Accepted) {
		return;
	}
	dialog.saveParameters();
	loadPersistentParameters();
}


// Help button handler.
void MainWindow::onHelpClicked()
{
	QMessageBox::information(this, _("Help"), _(
		"To start playing and then change the running side of the clock, just push "
		"some keys on the keyboard. The left player have to push some key on the left "
		"side of the keyboard, the right player on the right side.\n"
		"\n"
		"If it does not work, it might be due to a misdetection of your keyboard "
		"layout; you can check it and correct it through the \"Preferences\" dialog. "
		"If your actual keyboard layout is not available among those proposed by "
		"VCC, you should select \"Default QWERTY\" or one of the default layouts that "
		"suits good to your keyboard. Thus, only the A-Z keys will be used by VCC, "
		"but the software will be usable anyway."
	));
}


// Debug button handler.
void MainWindow::onDebugClicked()
{
	if(_debugDialog==nullptr) {
		_debugDialog = new DebugDialog(this);
	}
	_debugDialog->show();
}


// About button handler.
void MainWindow::onAboutClicked()
{
	///TODO: customize the about dialog.
	QMessageBox::about(this, "About VCC", "A simple and free chess clock software");
}


// Load the persistent parameters saved in the singleton Param object.
void MainWindow::loadPersistentParameters()
{
	// Keyboard shortcuts
	_shortcutManager.reset(Params::get().modifier_keys(),
		Params::get().keyboard_map(Params::get().current_keyboard()),
		Params::get().shortcut_map("FR") // TODO
	);

	// Reset confirmation option
	_resetConfirmation = Params::get().reset_confirmation();

	// Status bar
	_statusBar->setVisible(Params::get().show_status_bar());

	// Display options
	_biTimerWidget->setDelayBeforeDisplaySeconds(Params::get().delay_before_display_seconds());
	_biTimerWidget->setDisplayTimeAfterTimeout  (Params::get().display_time_after_timeout  ());
	_biTimerWidget->setDisplayBronsteinExtraInfo(Params::get().display_bronstein_extra_info());
	_biTimerWidget->setDisplayByoYomiExtraInfo  (Params::get().display_byo_yomi_extra_info ());
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

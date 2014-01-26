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
#include <wrappers/translation.h>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QToolButton>
#include <QMenu>
#include <QMessageBox>
#include <QEvent>
#include <QApplication>


// Constructor.
MainWindow::MainWindow() : _debugDialog(nullptr)
{
	setWindowTitle(QString::fromStdString(Params::get().app_full_name()));
	setWindowIcon(fetchIcon("logo", false));
	qApp->installEventFilter(this);

	// Low-level keyboard handler
	_keyboardHandler = new KeyboardHandler(this);
	connect(_keyboardHandler, &KeyboardHandler::keyPressed, this, &MainWindow::onKeyPressed);

	// Tool-bar timer.
	_toolBarTimer = new QTimer(this);
	_toolBarTimer->setInterval(500);
	_toolBarTimer->setSingleShot(true);
	connect(_toolBarTimer, &QTimer::timeout, this, &MainWindow::onTimeoutEvent);

	// Build the tool-bar.
	_toolBar = addToolBar(_("Main tool-bar"));
	_toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
	_toolBar->setFloatable(false);
	_toolBar->setMovable(false);
	QAction *actReset = _toolBar->addAction(fetchIcon("reset", false), _("Reset"         ));
	QAction *actPause = _toolBar->addAction(fetchIcon("pause", false), _("Pause"         ));
	QAction *actSwap  = _toolBar->addAction(fetchIcon("swap" , false), _("Swap sides"    ));
	_toolBar->addSeparator();
	QAction *actFlScr = _toolBar->addAction(fetchIcon("flscr", false), _("Full screen"   ));
	actFlScr->setCheckable(true);
	_toolBar->addSeparator();
	QAction *actTCtrl = _toolBar->addAction(fetchIcon("tctrl", false), _("Time control"  ));
	QAction *actNames = _toolBar->addAction(fetchIcon("names", false), _("Players' names"));
	_toolBar->addSeparator();
	QToolButton *btnMenu = new QToolButton(this);
	btnMenu->setText(_("Menu"));
	btnMenu->setAutoRaise(true);
	btnMenu->setFocusPolicy(Qt::NoFocus);
	_toolBar->addWidget(btnMenu);
	actReset->setToolTip(_("Reset the clock"                                 ));
	actPause->setToolTip(_("Pause the clock"                                 ));
	actSwap ->setToolTip(_("Swap the players and the time control parameters"));
	actFlScr->setToolTip(_("Full screen on/off"                              ));
	actTCtrl->setToolTip(_("Change the current time control"                 ));
	actNames->setToolTip(_("Edit the names of the players"                   ));
	btnMenu ->setToolTip(_("General options, help, etc..."                   ));

	// Build the menu.
	QMenu *menu = new QMenu(this);
	btnMenu->setMenu(menu);
	btnMenu->setPopupMode(QToolButton::InstantPopup);
	QAction *actPrefs = menu->addAction(fetchIcon("preferences-desktop"), _("Preferences"));
	menu->addSeparator();
	QAction *actDebug = menu->addAction(                                  _("Debug"      ));
	menu->addSeparator();
	QAction *actHelp  = menu->addAction(fetchIcon("help-contents"      ), _("Help"       ));
	QAction *actAbout = menu->addAction(fetchIcon("help-about"         ), _("About"      ));

	// Connect the action handlers.
	connect(actReset, &QAction::triggered, this, &MainWindow::onResetClicked);
	connect(actPause, &QAction::triggered, this, &MainWindow::onPauseClicked);
	connect(actSwap , &QAction::triggered, this, &MainWindow::onSwapClicked );
	connect(actTCtrl, &QAction::triggered, this, &MainWindow::onTCtrlClicked);
	connect(actFlScr, &QAction::triggered, this, &MainWindow::onFlScrClicked);
	connect(actNames, &QAction::triggered, this, &MainWindow::onNamesClicked);
	connect(actPrefs, &QAction::triggered, this, &MainWindow::onPrefsClicked);
	connect(actDebug, &QAction::triggered, this, &MainWindow::onDebugClicked);
	connect(actHelp , &QAction::triggered, this, &MainWindow::onHelpClicked );
	connect(actAbout, &QAction::triggered, this, &MainWindow::onAboutClicked);

	// Bi-timer widget
	_biTimerWidget = new BiTimerWidget(this);
	_biTimerWidget->bindTimer(_biTimer);
	setCentralWidget(_biTimerWidget);

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


// General event filter.
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
	if(event->type()==QEvent::MouseMove && isActiveWindow()) {
		onMouseMoveEvent();
	}
	return QMainWindow::eventFilter(object, event);
}


// Triggered when the mouse is moved.
void MainWindow::onMouseMoveEvent()
{
	if(!isFullScreen()) {
		return;
	}
	_toolBar->setVisible(true);
	_toolBarTimer->start();
}


// Triggered when the tool-bar timer elapses
void MainWindow::onTimeoutEvent()
{
	if(!isFullScreen()) {
		return;
	}
	_toolBar->setVisible(false);
}


// Key-press event handler.
void MainWindow::onKeyPressed(ScanCode scanCode)
{
	// Retrieve the shortcut that is triggered by the given scan-code, if any.
	bool modifierKeysActivated =
		_keyboardHandler->isDown(_shortcutManager.modifier_key(Side::LEFT )) &&
		_keyboardHandler->isDown(_shortcutManager.modifier_key(Side::RIGHT));
	int shortcut = _shortcutManager.shortcut(scanCode, modifierKeysActivated);

	// Execute the requested actions.
	switch(shortcut)
	{
		case 1: _biTimer.start_timer(Side::RIGHT); break;
		case 2: _biTimer.start_timer(Side::LEFT ); break;
		case 3: _biTimer.stop_timer  (); break;
		case 4: _biTimer.reset_timers(); break;
		case 5: onSwapClicked(); break;
		default: break;
	}
}


// Reset button handler.
void MainWindow::onResetClicked()
{
	if(_resetConfirmation==ResetConfirmation::ALWAYS || (_resetConfirmation==ResetConfirmation::IF_ACTIVE && _biTimer.is_active())) {
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


// Swap-sides button handler.
void MainWindow::onSwapClicked()
{
	// Swap the time control options.
	_biTimer.swap_sides();
	_statusBar->showMessage(QString::fromStdString(_biTimer.time_control().description()));
	Params::get().set_time_control(_biTimer.time_control());

	// Swap the players' names.
	QString name_buffer = _biTimerWidget->label(Side::LEFT);
	_biTimerWidget->setLabel(Side::LEFT , _biTimerWidget->label(Side::RIGHT));
	_biTimerWidget->setLabel(Side::RIGHT, name_buffer);
	Params::get().set_player_name(Side::LEFT , _biTimerWidget->label(Side::LEFT ).toStdString());
	Params::get().set_player_name(Side::RIGHT, _biTimerWidget->label(Side::RIGHT).toStdString());
}


// Full-screen button handler.
void MainWindow::onFlScrClicked()
{
	// If the window is in full-screen mode, leave it.
	if(isFullScreen()) {
		setWindowState(_previousState);
	}

	// Otherwise, save the current state, and enter the full-screen mode.
	else {
		_previousState = windowState();
		setWindowState(Qt::WindowFullScreen);
	}

	// Tool-bar and status bar
	_toolBar->setVisible(!isFullScreen());
	_statusBar->setVisible(!isFullScreen() && Params::get().show_status_bar());
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


// Debug button handler.
void MainWindow::onDebugClicked()
{
	if(_debugDialog==nullptr) {
		_debugDialog = new DebugDialog(this);
	}
	_debugDialog->show();
}


// Help button handler.
void MainWindow::onHelpClicked()
{
	QString link = QString("<a href=\"%1\">%1</a>").arg(_(
		"http://en.wikipedia.org/wiki/Time_control"
	));

	QString message;
	message += "<p>" + QString(_(
		"To start playing and then change the running side of the clock, "
		"just press some keys on the keyboard. "
		"The left player must press keys on the left side of the keyboard, "
		"the right player on the right side. "
		"The role of each key can be edited in the \"%1\" dialog, "
		"available through the menu \"%2\" &gt; \"%1\"."
	)).arg(_("Preferences")).arg(_("Menu")) + "</p>";
	message += "<p>" + _(
		"Most tool-bar buttons and dialog fields have tool-tips "
		"to give a short description of their respective roles. "
		"To show the tool-tip, just place the mouse cursor over the button or the field "
		"you want to be explained, and wait a few seconds."
	) + "</p>";
	message += "<p>" + QString(_(
		"General description of time control systems and rules can be found on the Internet, "
		"for instance: %1."
	)).arg(link) + "</p>";

	QMessageBox::information(this, _("Help"), message);
}


// About button handler.
void MainWindow::onAboutClicked()
{
	QString title = QString(_("About %1")).arg(QString::fromStdString(Params::get().app_full_name()));

	QString message;
	message += "<h1>" + QString::fromStdString(
		Params::get().app_full_name() + " " + Params::get().app_version().full_number()
	) + "</h1>";
	message += "<p>" + _("A simple and free chess clock software.") + "</p>";
	message += "<p>"
		"<a href=\"http://vchessclock.sourceforge.net/\">http://vchessclock.sourceforge.net/</a><br/>"
		"<a href=\"https://github.com/yo35/vcc\">https://github.com/yo35/vcc</a> (" + _("developer link") + ")" //TODO: register on GitHub
	"</p>";
	message += "<p><small><i>" + QString(_(
		"If you encounter some bugs with this program, or if you wish to get new features in the future versions, "
		"you can report/propose them in the bug tracker at %1."
	)).arg("<a href=\"https://github.com/yo35/vcc/issues\">https://github.com/yo35/vcc/issues</a>") + "</i></small></p>"; //TODO: register on GitHub
	message += "<h2>" + _("Author") + "</h2>";
	message += "<p>Yoann Le Montagner <a href=\"mailto:yo35@melix.net\">&lt;yo35@melix.net&gt;</a></p>";
	message += "<h2>" + _("Translators") + "</h2>";
	message += "<table><tbody>";
	message += "<tr><td>Deutsch</td><td>Tobias Küchel</td></tr>";
	message += "<tr><td>English</td><td>Yoann Le Montagner</td></tr>";
	message += "<tr><td>Español</td><td>Santiago Paz</td></tr>";
	message += "<tr><td>Français</td><td>Yoann Le Montagner</td></tr>";
	message += "</tbody></table>";
	message += "<p><small><i>" + _(
		"If you are interested in translating this plugin into your language, "
		"please contact the author."
	) + "</i></small></p>";
	message += "<h2>" + _("License") + "</h2>";
	message += "<p>" + _(
		"This program is free software: you can redistribute it and/or modify "
		"it under the terms of the GNU General Public License as published by "
		"the Free Software Foundation, either version 3 of the License, or "
		"(at your option) any later version."
	) + "</p>";
	message += "<p>" + _(
		"This program is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		"GNU General Public License for more details."
	) + "</p>";
	message += "<p>" + _(
		"You should have received a copy of the GNU General Public License "
		"along with this program. If not, see "
		"<a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>."
	) + "</p>";
	message += "<p>Copyright (C) 2010-2014 Yoann Le Montagner.</p>";

	QMessageBox::about(this, title, message);
}


// Load the persistent parameters saved in the singleton Param object.
void MainWindow::loadPersistentParameters()
{
	// Keyboard shortcuts
	_shortcutManager.reset(Params::get().modifier_keys(),
		Params::get().keyboard_map(Params::get().current_keyboard()),
		Params::get().shortcut_map()
	);

	// Reset confirmation option
	_resetConfirmation = Params::get().reset_confirmation();

	// Status bar
	_statusBar->setVisible(!isFullScreen() && Params::get().show_status_bar());

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

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


#include "mainwindow.h"
#include "preferencedialog.h"

#include <wrappers/translation.h>

#include <models/modelpaths.h>
#include <models/modelappinfo.h>
#include <models/modelkeyboard.h>
#include <models/modelshortcutmap.h>
#include <models/modelmain.h>

#include <gui/core/keyboardhandler.h>
#include <gui/widgets/bitimerwidget.h>
#include <gui/dialogs/timecontroldialog.h>
#include <gui/dialogs/namedialog.h>
#include <gui/dialogs/debugdialog.h>

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QMenu>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>


// Constructor.
MainWindow::MainWindow() : _debugDialog(nullptr)
{
	ModelAppInfo &appInfo(ModelAppInfo::instance());
	setWindowTitle(QString::fromStdString(appInfo.full_name()));
	setWindowIcon(appInfo.icon());
	qApp->installEventFilter(this);

	// Low-level keyboard handler
	_keyboardHandler = new KeyboardHandler(this);
	connect(_keyboardHandler, &KeyboardHandler::keyPressed, this, &MainWindow::onKeyPressed);

	// Tool-bar timer.
	_toolBarTimer = new QTimer(this);
	_toolBarTimer->setInterval(500);
	_toolBarTimer->setSingleShot(true);
	connect(_toolBarTimer, &QTimer::timeout, this, &MainWindow::onToolbarTimerElapsed);

	// Build the tool-bar.
	_toolBar = addToolBar(_("Main tool-bar"));
	_toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
	_toolBar->setFloatable(false);
	_toolBar->setMovable(false);
	QAction *actReset = _toolBar->addAction(appInfo.toolbar_icon("reset"), _("Reset"         ));
	QAction *actPause = _toolBar->addAction(appInfo.toolbar_icon("pause"), _("Pause"         ));
	QAction *actSwap  = _toolBar->addAction(appInfo.toolbar_icon("swap" ), _("Swap sides"    ));
	_toolBar->addSeparator();
	QAction *actFlScr = _toolBar->addAction(appInfo.toolbar_icon("flscr"), _("Full screen"   ));
	actFlScr->setCheckable(true);
	_toolBar->addSeparator();
	QAction *actTCtrl = _toolBar->addAction(appInfo.toolbar_icon("tctrl"), _("Time control"  ));
	QAction *actNames = _toolBar->addAction(appInfo.toolbar_icon("names"), _("Players' names"));
	_toolBar->addSeparator();
	QToolButton *btnMenu = new QToolButton(this);
	btnMenu->setText(_("Menu"));
	btnMenu->setAutoRaise(true);
	btnMenu->setFocusPolicy(Qt::NoFocus);
	_toolBar->addWidget(btnMenu);
	actReset->setToolTip(_("Reset the clock"                                        ));
	actPause->setToolTip(_("Pause the clock"                                        ));
	actSwap ->setToolTip(_("Swap the players' names and the time control parameters"));
	actFlScr->setToolTip(_("Full screen on/off"                                     ));
	actTCtrl->setToolTip(_("Change the current time control"                        ));
	actNames->setToolTip(_("Edit the names of the players"                          ));
	btnMenu ->setToolTip(_("General options, help, etc..."                          ));

	// Build the menu.
	QMenu *menu = new QMenu(this);
	btnMenu->setMenu(menu);
	btnMenu->setPopupMode(QToolButton::InstantPopup);
	QAction *actPrefs = menu->addAction(QIcon::fromTheme("preferences-desktop"), _("Preferences"));
	menu->addSeparator();
	QAction *actDebug = menu->addAction(                                         _("Debug"      ));
	menu->addSeparator();
	QAction *actHelp  = menu->addAction(QIcon::fromTheme("help-contents"      ), _("Help"       ));
	QAction *actAbout = menu->addAction(QIcon::fromTheme("help-about"         ), _("About"      ));

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

	// Main model
	ModelMain &model(ModelMain::instance());

	// Bi-timer widget
	_biTimerWidget = new BiTimerWidget(this);
	_biTimerWidget->bindTimer(_biTimer);
	setCentralWidget(_biTimerWidget);
	model.delay_before_display_seconds.connect_changed(std::bind(&BiTimerWidget::setDelayBeforeDisplaySeconds, _biTimerWidget, std::placeholders::_1));
	model.display_time_after_timeout  .connect_changed(std::bind(&BiTimerWidget::setDisplayTimeAfterTimeout  , _biTimerWidget, std::placeholders::_1));
	model.display_bronstein_extra_info.connect_changed(std::bind(&BiTimerWidget::setDisplayBronsteinExtraInfo, _biTimerWidget, std::placeholders::_1));
	model.display_byo_yomi_extra_info .connect_changed(std::bind(&BiTimerWidget::setDisplayByoYomiExtraInfo  , _biTimerWidget, std::placeholders::_1));
	model.show_player_names           .connect_changed(std::bind(&BiTimerWidget::setShowLabels               , _biTimerWidget, std::placeholders::_1));
	model.left_player .connect_changed(std::bind(&BiTimerWidget::setLabel, _biTimerWidget, Side::LEFT , std::placeholders::_1));
	model.right_player.connect_changed(std::bind(&BiTimerWidget::setLabel, _biTimerWidget, Side::RIGHT, std::placeholders::_1));
	_biTimerWidget->setDelayBeforeDisplaySeconds(model.delay_before_display_seconds());
	_biTimerWidget->setDisplayTimeAfterTimeout  (model.display_time_after_timeout  ());
	_biTimerWidget->setDisplayBronsteinExtraInfo(model.display_bronstein_extra_info());
	_biTimerWidget->setDisplayByoYomiExtraInfo  (model.display_byo_yomi_extra_info ());
	_biTimerWidget->setShowLabels               (model.show_player_names           ());
	_biTimerWidget->setLabel(Side::LEFT , model.left_player ());
	_biTimerWidget->setLabel(Side::RIGHT, model.right_player());

	// Status bar
	_statusBar = statusBar();
	model.show_status_bar.connect_changed(std::bind(&MainWindow::refreshStatusBarVisibility, this));
	refreshStatusBarVisibility();

	// Load the time control
	model.time_control.connect_changed(std::bind(&MainWindow::refreshTimeControl, this));
	refreshTimeControl();

	// Initialize the shortcut manager.
	refreshShortcutManager();
}


// Close event handler.
void MainWindow::closeEvent(QCloseEvent *)
{
	ModelMain::instance().save();
	ModelShortcutMap::instance().save();
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
void MainWindow::onToolbarTimerElapsed()
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
	ResetConfirmation mode = ModelMain::instance().reset_confirmation();
	if(mode==ResetConfirmation::ALWAYS || (mode==ResetConfirmation::IF_ACTIVE && _biTimer.is_active())) {
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
	ModelMain &model(ModelMain::instance());

	// Swap the time control options.
	_biTimer.swap_sides();
	model.time_control(_biTimer.time_control());

	// Swap the players' names.
	QString name_buffer = model.left_player();
	model.left_player(model.right_player());
	model.right_player(name_buffer);
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
	refreshStatusBarVisibility();
}


// Time control button handler.
void MainWindow::onTCtrlClicked()
{
	ModelMain &model(ModelMain::instance());
	TimeControlDialog dialog(this);
	dialog.setTimeControl(model.time_control());
	if(dialog.exec()!=QDialog::Accepted) {
		return;
	}
	model.time_control(dialog.timeControl());
}


// Players' names button handler.
void MainWindow::onNamesClicked()
{
	ModelMain &model(ModelMain::instance());
	NameDialog dialog(this);
	dialog.setName(Side::LEFT , model.left_player ());
	dialog.setName(Side::RIGHT, model.right_player());
	dialog.setDisplayNames(model.show_player_names());
	if(dialog.exec()!=QDialog::Accepted) {
		return;
	}
	model.left_player (dialog.name(Side::LEFT ));
	model.right_player(dialog.name(Side::RIGHT));
	model.show_player_names(dialog.displayNames());
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
	refreshShortcutManager();
}


// Debug button handler.
void MainWindow::onDebugClicked()
{
	if(_debugDialog==nullptr) {
		_debugDialog = new DebugDialog(this);
		_debugDialog->addInfo("Share path" , QString::fromStdString(ModelPaths  ::instance().share_path ()));
		_debugDialog->addInfo("Config path", QString::fromStdString(ModelPaths  ::instance().config_path()));
		_debugDialog->addInfo("Locale"     , QString::fromStdString(ModelAppInfo::instance().locale     ()));
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
	ModelAppInfo &model(ModelAppInfo::instance());
	QString title = QString(_("About %1")).arg(QString::fromStdString(model.full_name()));
	QString message;

	message += "<h1>" + QString::fromStdString(model.full_name() + " " + model.version().full_number()) + "</h1>";
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
		"If you are interested in translating this program into your language, "
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


// Refresh the time control.
void MainWindow::refreshTimeControl()
{
	ModelMain &model(ModelMain::instance());
	_biTimer.set_time_control(model.time_control());
	_statusBar->showMessage(QString::fromStdString(model.time_control().description()));
}


// Show or hide the status bar.
void MainWindow::refreshStatusBarVisibility()
{
	_statusBar->setVisible(!isFullScreen() && ModelMain::instance().show_status_bar());
}


// Refresh the shortcut manager based on the keyboard settings.
void MainWindow::refreshShortcutManager()
{
	_shortcutManager.reset(
		ModelMain::instance().modifier_keys(),
		ModelKeyboard::instance().keyboard_map(ModelMain::instance().keyboard_id()),
		ModelShortcutMap::instance().shortcut_map()
	);
}

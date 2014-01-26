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


#include "debugdialog.h"
#include <wrappers/translation.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>


// Constructor.
DebugDialog::DebugDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(_("Debug information"));
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	// Keyboard handler.
	_keyboardHandler = new KeyboardHandler(this);
	connect(_keyboardHandler, &KeyboardHandler::keyPressed , this, &DebugDialog::onKeyPressed );
	connect(_keyboardHandler, &KeyboardHandler::keyReleased, this, &DebugDialog::onKeyReleased);

	// Text widget.
	_info = new QTextEdit(this);
	_info->setReadOnly(true);
	layout->addWidget(new QLabel(_("Press some key..."), this));
	layout->addWidget(_info, 1);
}


// Default size.
QSize DebugDialog::sizeHint() const
{
	return QSize(400, 400);
}


// Window state-change handler.
void DebugDialog::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::ActivationChange) {
		_keyboardHandler->setEnabled(isActiveWindow());
	}
	QDialog::changeEvent(event);
}


// Key-pressed handler.
void DebugDialog::onKeyPressed (ScanCode scanCode)
{
	_info->append(QString("<b style=\"color: #c00000;\">Key pressed</b>, code=%1").arg(scanCode));
}


// Key-released handler.
void DebugDialog::onKeyReleased(ScanCode scanCode)
{
	_info->append(QString("<b style=\"color: #008000;\">Key released</b>, code=%1").arg(scanCode));
}

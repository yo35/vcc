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
#include <translation.h>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>


// Constructor.
DebugDialog::DebugDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(_("Debug information"));
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	// Key information
	QGroupBox *keyInfoGroup = new QGroupBox(_("Key pressed"), this);
	mainLayout->addWidget(keyInfoGroup);
	QGridLayout *keyInfoLayout = new QGridLayout;
	keyInfoGroup->setLayout(keyInfoLayout);

	// Key information fields
	_infoKeyID       = new QLineEdit(this);
	_infoKeyText     = new QLineEdit(this);
	_infoKeyNativeID = new QLineEdit(this);
	_infoKeyScanCode = new QLineEdit(this);
	_infoKeyID      ->setReadOnly(true);
	_infoKeyText    ->setReadOnly(true);
	_infoKeyNativeID->setReadOnly(true);
	_infoKeyScanCode->setReadOnly(true);
	keyInfoLayout->addWidget(new QLabel(_("Key ID"       )), 0, 0, Qt::AlignCenter);
	keyInfoLayout->addWidget(new QLabel(_("Key text"     )), 1, 0, Qt::AlignCenter);
	keyInfoLayout->addWidget(new QLabel(_("Native key ID")), 0, 2, Qt::AlignCenter);
	keyInfoLayout->addWidget(new QLabel(_("Scan code"    )), 1, 2, Qt::AlignCenter);
	keyInfoLayout->addWidget(_infoKeyID      , 0, 1);
	keyInfoLayout->addWidget(_infoKeyText    , 1, 1);
	keyInfoLayout->addWidget(_infoKeyNativeID, 0, 3);
	keyInfoLayout->addWidget(_infoKeyScanCode, 1, 3);
	keyInfoLayout->setColumnStretch(1, 1);
	keyInfoLayout->setColumnStretch(3, 1);

	// Dialog geometry
	mainLayout->addStretch(1);
}


// Key-press event handler.
void DebugDialog::keyPressEvent(QKeyEvent *event)
{
	if(event->isAutoRepeat()) {
		return;
	}
	_infoKeyID      ->setText(QString::number(event->key()));
	_infoKeyText    ->setText(event->text());
	_infoKeyNativeID->setText(QString::number(event->nativeVirtualKey()));
	_infoKeyScanCode->setText(QString::number(event->nativeScanCode  ()));
}

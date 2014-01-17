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


#include "namedialog.h"
#include <translation.h>
#include <QLabel>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>


// Constructor.
NameDialog::NameDialog(QWidget *parent) : QDialog(parent)
{
	// Top-level layout
	setWindowTitle(_("Players' names"));
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	// Name fields
	_name[Side::LEFT ] = new QLineEdit(this);
	_name[Side::RIGHT] = new QLineEdit(this);
	QGridLayout *nameLayout = new QGridLayout;
	layout->addLayout(nameLayout);
	nameLayout->addWidget(new QLabel(_("Left player" ), this), 0, 0);
	nameLayout->addWidget(new QLabel(_("Right player"), this), 0, 1);
	nameLayout->addWidget(_name[Side::LEFT ], 1, 0);
	nameLayout->addWidget(_name[Side::RIGHT], 1, 1);

	// Button and check-box
	_switch    = new QPushButton(_("Switch players"         ), this);
	_showNames = new QCheckBox  (_("Show the players' names"), this);
	QHBoxLayout *ctrlLayout = new QHBoxLayout;
	layout->addLayout(ctrlLayout);
	ctrlLayout->addWidget(_switch);
	ctrlLayout->addWidget(_showNames, 1);
	connect(_showNames, &QCheckBox::toggled, this, &NameDialog::refreshActivationState);
	refreshActivationState();

	// Validation buttons
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	layout->addStretch(1);
	layout->addWidget(buttons);
}


// Name selected for the player on the given side.
QString NameDialog::name(Side side) const
{
	return _name[side]->text();
}


// Set the name of the player on the given side.
void NameDialog::setName(Side side, const QString &value)
{
	_name[side]->setText(value);
}


// Whether the names should be visible or not.
bool NameDialog::displayNames() const
{
	return _showNames->isChecked();
}


// Set the default choice for name visibility.
void NameDialog::setDisplayNames(bool value)
{
	_showNames->setChecked(value);
}


// Activate/dis-activate the name fields and the switch button when names are
// respectively shown/hidden.
void NameDialog::refreshActivationState()
{
	bool enabled = _showNames->isChecked();
	_name[Side::LEFT ]->setEnabled(enabled);
	_name[Side::RIGHT]->setEnabled(enabled);
	_switch->setEnabled(enabled);
}

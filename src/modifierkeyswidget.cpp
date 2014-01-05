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


#include "modifierkeyswidget.h"
#include <translation.h>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <QMenu>


// Constructor.
ModifierKeysWidget::ModifierKeysWidget(QWidget *parent) :
	CaptionWidget(QBrush(QColor(0,0,128), Qt::FDiagPattern), _("Modifier keys"), parent)
{
	// Prepare the composition of the menu labels.
	static boost::format labelFormater(_("%1% keys (left and right)"));
	Enum::array<ModifierKeys, std::string> name;
	name[ModifierKeys::DOUBLE_CTRL ] = _("Ctrl" );
	name[ModifierKeys::DOUBLE_SHIFT] = _("Shift");
	name[ModifierKeys::DOUBLE_ALT  ] = _("Alt"  );

	// Create the menu
	QMenu *menu = new QMenu(this);
	QActionGroup *group = new QActionGroup(this);
	group->setExclusive(true);
	for(auto it=Enum::cursor<ModifierKeys>::first(); it.valid(); ++it) {
		_action[*it] = menu->addAction(QString::fromStdString(boost::str(labelFormater.clear() % name[*it])));
		_action[*it]->setCheckable(true);
		_action[*it]->setActionGroup(group);
		connect(_action[*it], &QAction::triggered, boost::bind(&ModifierKeysWidget::onActionTriggered, this, *it));
	}

	// Default value
	_modifierKeys = ModifierKeys::DOUBLE_CTRL;
	_action[_modifierKeys]->setChecked(true);

	// Set the menu
	button()->setMenu(menu);
	button()->setPopupMode(QToolButton::InstantPopup);
	setText(text() + " ");
}


// Change the selected modifier keys.
void ModifierKeysWidget::setModifierKeys(ModifierKeys value)
{
	if(_modifierKeys==value) {
		return;
	}
	_modifierKeys = value;
	_action[_modifierKeys]->setChecked(true);
	emit valueChanged();
}


// Action triggered when one of the menu item is clicked.
void ModifierKeysWidget::onActionTriggered(ModifierKeys value)
{
	if(_modifierKeys==value) {
		return;
	}
	_modifierKeys = value;
	emit valueChanged();
}

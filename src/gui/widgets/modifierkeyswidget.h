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


#ifndef MODIFIERKEYSWIDGET_H_
#define MODIFIERKEYSWIDGET_H_

#include "captionwidget.h"
#include <core/keys.h>
#include <QAction>


/**
 * Caption widget with a popup menu to select a pair of modifier keys.
 */
class ModifierKeysWidget : public CaptionWidget
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	ModifierKeysWidget(QWidget *parent=0);

	/**
	 * Return the selected modifier keys.
	 */
	ModifierKeys modifierKeys() const { return _modifierKeys; }

	/**
	 * Change the selected modifier keys.
	 */
	void setModifierKeys(ModifierKeys value);

signals:

	/**
	 * Signal emitted when the selected modifier keys changes.
	 */
	void valueChanged();

private:

	// Private function
	void onActionTriggered(ModifierKeys value);
	static QBrush buildBrush();

	// Private members
	ModifierKeys _modifierKeys;
	Enum::array<ModifierKeys, QAction *> _action;
};

#endif /* MODIFIERKEYSWIDGET_H_ */

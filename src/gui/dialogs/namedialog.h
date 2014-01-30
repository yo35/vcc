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


#ifndef NAMEDIALOG_H_
#define NAMEDIALOG_H_

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <core/side.h>


/**
 * Dialog to edit the name of the players.
 */
class NameDialog : public QDialog
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	NameDialog(QWidget *parent=0);

	/**
	 * Name selected for the player on the given side.
	 */
	QString name(Side side) const;

	/**
	 * Set the name of the player on the given side.
	 */
	void setName(Side side, const QString &value);

	/**
	 * Whether the names should be visible or not.
	 */
	bool displayNames() const;

	/**
	 * Set the default choice for name visibility.
	 */
	void setDisplayNames(bool value);

private:

	// Private functions
	void refreshActivationState();

	// Private members
	Enum::array<Side, QLineEdit *> _name     ;
	QCheckBox                     *_showNames;
};

#endif /* NAMEDIALOG_H_ */

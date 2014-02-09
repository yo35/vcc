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


#ifndef DEBUGDIALOG_H_
#define DEBUGDIALOG_H_

#include <QDialog>
#include <core/keys.h>

QT_BEGIN_NAMESPACE
	class QTextEdit;
QT_END_NAMESPACE

class KeyboardHandler;


/**
 * Dialog providing extra-information for debugging purposes.
 */
class DebugDialog : public QDialog
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	DebugDialog(QWidget *parent=0);

	/**
	 * Add some information to the text field.
	 */
	void addInfo(const QString &name, const QString &value);

	/**
	 * Default size.
	 */
	QSize sizeHint() const override;

protected:

	/**
	 * Window state-change handler.
	 */
	void changeEvent(QEvent *event) override;

private:

	// Private functions
	void onKeyPressed (ScanCode scanCode);
	void onKeyReleased(ScanCode scanCode);

	// Private members
	KeyboardHandler *_keyboardHandler;
	QTextEdit       *_info;
};

#endif /* DEBUGDIALOG_H_ */

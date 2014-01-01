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


#ifndef PREFERENCEDIALOG_H_
#define PREFERENCEDIALOG_H_

#include <QDialog>
QT_BEGIN_NAMESPACE
	class QCheckBox;
QT_END_NAMESPACE
class KeyboardHandler;
class KeyboardMapWidget;


/**
 * Dialog to edit user's preferences.
 */
class PreferenceDialog : public QDialog
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	PreferenceDialog(const KeyboardHandler *keyboardHandler, QWidget *parent=0);

	/**
	 * Load the dialog with the parameters saved in the Params singleton object.
	 */
	void loadParameters();

	/**
	 * Save the new parameters defined from the dialog in the Params singleton object.
	 */
	void saveParameters();

private:

	// Private functions
	QWidget *createKeyboardPage();
	QWidget *createDisplayPage ();
	void onDisplayNumericKeypadToggled();

	// Keyboard page
	const KeyboardHandler *_keyboardHandler     ;
	QCheckBox             *_displayNumericKeypad;
	KeyboardMapWidget     *_keyboardMapWidget   ;

	// Display page
	QCheckBox *_displayStatusBar         ;
	QCheckBox *_displayTimeAfterFlagDown ;
	QCheckBox *_displayBronsteinExtraTime;
	QCheckBox *_displayByoYomiExtraTime  ;
};

#endif /* PREFERENCEDIALOG_H_ */

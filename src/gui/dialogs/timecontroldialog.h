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


#ifndef TIMECONTROLDIALOG_H_
#define TIMECONTROLDIALOG_H_

#include <QDialog>
#include <core/timecontrol.h>

QT_BEGIN_NAMESPACE
	class QRadioButton;
	class QCheckBox;
	class QSpinBox;
QT_END_NAMESPACE

class TimeDurationWidget;


/**
 * Dialog to edit the time control rules.
 */
class TimeControlDialog : public QDialog
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	TimeControlDialog(QWidget *parent=0);

	/**
	 * Return the time control defined in the dialog.
	 */
	TimeControl timeControl() const;

	/**
	 * Initialize the time control in the dialog.
	 */
	void setTimeControl(const TimeControl &value);

private:

	// Private functions
	void onModeButtonToggled(QRadioButton *button);
	void onIdenticTimesToggled();
	void refreshEnableStates  ();
	void copyLeftMainTime     ();
	void copyLeftIncrement    ();
	void copyLeftByoPeriods   ();

	// Private members
	bool _shuntSignal;
	Enum::array<TimeControl::Mode, QRadioButton *> _mode        ;
	QCheckBox *                                    _identicTimes;
	Enum::array<Side, TimeDurationWidget *>        _mainTime    ;
	Enum::array<Side, TimeDurationWidget *>        _increment   ;
	Enum::array<Side, QSpinBox           *>        _byoPeriods  ;
};

#endif /* TIMECONTROLDIALOG_H_ */

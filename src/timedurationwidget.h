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


#ifndef TIMEDURATIONWIDGET_H_
#define TIMEDURATIONWIDGET_H_

#include <QWidget>
#include "chrono.h"
QT_BEGIN_NAMESPACE
	class QSpinBox;
QT_END_NAMESPACE


/**
 * Widget to display and edit a time duration.
 */
class TimeDurationWidget : public QWidget
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	TimeDurationWidget(QWidget *parent=0);

	/**
	 * Value displayed in the widget.
	 */
	TimeDuration timeDuration() const;

	/**
	 * Change the value displayed in the widget.
	 */
	void setTimeDuration(const TimeDuration &value);

signals:

	/**
	 * Signal emitted when the time duration value in the widget changes.
	 */
	void valueChanged();

private:

	// Private function
	void onFieldChanged(int v);

	// Private members
	bool _shuntSignal;
	QSpinBox *_hrs;
	QSpinBox *_min;
	QSpinBox *_sec;
};

#endif /* TIMEDURATIONWIDGET_H_ */

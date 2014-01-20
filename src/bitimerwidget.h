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


#ifndef BITIMERWIDGET_H_
#define BITIMERWIDGET_H_

#include <QWidget>
#include <memory>
#include "bitimer.h"
QT_BEGIN_NAMESPACE
	class QTimer;
QT_END_NAMESPACE


/**
 * Display a BiTimer object.
 */
class BiTimerWidget : public QWidget
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	BiTimerWidget(QWidget *parent=0);

	/**
	 * Check whether a timer is binded to the widget or not.
	 */
	bool hasTimerBinded() const { return _biTimer!=nullptr; }

	/**
	 * Return a reference to the binded bi-timer object.
	 * @throw std::invalid_argument If no timer is binded to the widget.
	 */
	const BiTimer &biTimer() const { ensureTimerBinded(); return *_biTimer; }

	/**
	 * Bind a timer to the widget.
	 */
	void bindTimer(const BiTimer &biTimer);

	/**
	 * Unbind the timer currently binded, if any.
	 */
	void unbindTimer();

	/**
	 * @name Size hint methods.
	 * @{
	 */
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	/**@} */

protected:

	/**
	 * Widget rendering method.
	 */
	void paintEvent(QPaintEvent *event) override;

private:

	// Private functions
	void ensureTimerBinded() const;
	void onTimerStateChanged();
	void onTimeoutEvent();

	// Private members
	std::unique_ptr<sig::scoped_connection> _connection;
	const BiTimer                          *_biTimer   ;
	QTimer                                 *_timer     ;
};

#endif /* BITIMERWIDGET_H_ */

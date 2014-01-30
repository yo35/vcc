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


#ifndef BITIMERWIDGET_H_
#define BITIMERWIDGET_H_

#include <QWidget>
#include <memory>
#include <core/bitimer.h>

QT_BEGIN_NAMESPACE
	class QTimer;
	class QPainter;
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
	 * Left or right label (typically the name of the corresponding player).
	 */
	const QString &label(Side side) const { return _label[side]; }

	/**
	 * Set the left or right label.
	 */
	void setLabel(Side side, const QString &value);

	/**
	 * Whether the labels are displayed or not.
	 */
	bool showLabels() const { return _showLabels; }

	/**
	 * Set whether the labels are displayed or not.
	 */
	void setShowLabels(bool value);

	/**
	 * Minimal remaining time before seconds is displayed.
	 */
	const TimeDuration &delayBeforeDisplaySeconds() const { return _delayBeforeDisplaySeconds; }

	/**
	 * Set the minimal remaining time before seconds is displayed.
	 */
	void setDelayBeforeDisplaySeconds(const TimeDuration &value);

	/**
	 * Whether the time should be displayed after timeout.
	 */
	bool displayTimeAfterTimeout() const { return _displayTimeAfterTimeout; }

	/**
	 * Set whether the time should be displayed after timeout.
	 */
	void setDisplayTimeAfterTimeout(bool value);

	/**
	 * Whether extra-information is displayed in Bronstein-mode.
	 */
	bool displayBronsteinExtraInfo() const { return _displayBronsteinExtraInfo; }

	/**
	 * Set whether extra-information is displayed in Bronstein-mode.
	 */
	void setDisplayBronsteinExtraInfo(bool value);

	/**
	 * Whether extra-information is displayed in byo-yomi-mode.
	 */
	bool displayByoYomiExtraInfo() const { return _displayByoYomiExtraInfo; }

	/**
	 * Set whether extra-information is displayed in byo-yomi-mode.
	 */
	void setDisplayByoYomiExtraInfo(bool value);

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
	void drawText(double x, double y, double w, double h, Qt::Alignment flags, const QString &text);
	void applyFontFactor(double factor);
	double computeFontFactor(double w, double h, const QString &text) const;
	QString timeDurationAsString(const TimeDuration &value) const;

	// Private members
	std::unique_ptr<sig::scoped_connection> _connection;
	QTimer                                 *_timer     ;
	const BiTimer                          *_biTimer   ;
	Enum::array<Side, QString>              _label     ;
	bool         _showLabels               ;
	TimeDuration _delayBeforeDisplaySeconds;
	bool         _displayTimeAfterTimeout  ;
	bool         _displayBronsteinExtraInfo;
	bool         _displayByoYomiExtraInfo  ;

	// Temporary members used at rendering time
	// (should not be used outside the `paintEvent` method).
	QPainter *_painter;
};

#endif /* BITIMERWIDGET_H_ */

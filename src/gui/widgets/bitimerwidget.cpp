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


#include "bitimerwidget.h"
#include <wrappers/translation.h>
#include <QPainter>
#include <QTimer>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <functional>


// Constructor.
BiTimerWidget::BiTimerWidget(QWidget *parent) : QWidget(parent), _biTimer(nullptr),
	_showLabels(false), _delayBeforeDisplaySeconds(from_seconds(3600)),
	_displayTimeAfterTimeout(true), _displayBronsteinExtraInfo(true), _displayByoYomiExtraInfo(true),
	_painter(nullptr)
{
	_timer = new QTimer(this);
	_timer->setInterval(100);
	connect(_timer, &QTimer::timeout, this, &BiTimerWidget::onTimeoutEvent);
	_timer->start();
}


// Throw an exception if no timer is binded to the widget.
void BiTimerWidget::ensureTimerBinded() const
{
	if(_biTimer==nullptr) {
		throw std::invalid_argument("No timer is currently binded to the widget.");
	}
}


// Bind a timer to the widget.
void BiTimerWidget::bindTimer(const BiTimer &biTimer)
{
	// If both the BiTimer object is unchanged, nothing to do.
	if(_biTimer==&biTimer) {
		return;
	}

	// Perform the suitable connection/disconnection operations.
	auto rawConnection = biTimer.connect_state_changed(std::bind(&BiTimerWidget::onTimerStateChanged, this));
	_connection.reset(new sig::scoped_connection(rawConnection));
	_biTimer = &biTimer;

	// Refresh the widget.
	update();
}


// Un-bind the timer currently binded, if any.
void BiTimerWidget::unbindTimer()
{
	// Nothing to do if no timer is currently binded.
	if(_biTimer==nullptr) {
		return;
	}

	// Otherwise, disconnect the timer and refresh the widget.
	_connection.reset();
	_biTimer = nullptr;
	update();
}


// Set the left or right label.
void BiTimerWidget::setLabel(Side side, const QString &value)
{
	_label[side] = value;
	update();
}


// Set whether the labels are displayed or not.
void BiTimerWidget::setShowLabels(bool value)
{
	_showLabels = value;
	update();
}


// Set the minimal remaining time before seconds is displayed.
void BiTimerWidget::setDelayBeforeDisplaySeconds(const TimeDuration &value)
{
	_delayBeforeDisplaySeconds = value;
	update();
}


// Set whether the time should be displayed after timeout.
void BiTimerWidget::setDisplayTimeAfterTimeout(bool value)
{
	_displayTimeAfterTimeout = value;
	update();
}


// Set whether extra-information is displayed in Bronstein-mode.
void BiTimerWidget::setDisplayBronsteinExtraInfo(bool value)
{
	_displayBronsteinExtraInfo = value;
	update();
}


// Set whether extra-information is displayed in byo-yomi-mode.
void BiTimerWidget::setDisplayByoYomiExtraInfo(bool value)
{
	_displayByoYomiExtraInfo = value;
	update();
}


// Size hints.
QSize BiTimerWidget::minimumSizeHint() const { return QSize(500, 200); }
QSize BiTimerWidget::sizeHint       () const { return QSize(800, 300); }


// Handler for the timer state-change event.
void BiTimerWidget::onTimerStateChanged()
{
	update();
}


// Handler periodically called by the internal QTimer object.
void BiTimerWidget::onTimeoutEvent()
{
	if(_biTimer==nullptr || !_biTimer->is_active()) {
		return;
	}
	update();
}


// Widget rendering method.
void BiTimerWidget::paintEvent(QPaintEvent *)
{
	// Create the painter object.
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);

	// Special paint for the case where no timer is binded: draw a red cross.
	if(_biTimer==nullptr) {
		painter.setBrush(Qt::white);
		painter.drawRect(0, 0, width(), height());
		QPen crossPen(Qt::red);
		crossPen.setWidth(10);
		painter.setPen(crossPen);
		painter.drawLine(0, 0, width()-1, height()-1);
		painter.drawLine(0, height()-1, width()-1, 0);
		return;
	}

	// Prepare the rendering of left and right areas.
	Enum::array<Side, double> x, w;
	x[Side::LEFT ] = 0            ; w[Side::LEFT ] = std::round(width()/2);
	x[Side::RIGHT] = w[Side::LEFT]; w[Side::RIGHT] = width() - w[Side::LEFT];
	double y = 0;
	double h = height();
	_painter = &painter;

	// Background.
	for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it) {
		_painter->setBrush(_biTimer->active_side()==*it ? QColor(255,255,128) : Qt::white);
		_painter->drawRect(x[*it], y, w[*it], h);
	}

	// Color to use for the text.
	Enum::array<Side, QColor> textColor;

	// Time rendering.
	for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it) {
		BiTimer::TimeInfo info = _biTimer->detailed_time(*it);
		bool isNegative = info.total_time<TimeDuration::zero();

		// Color to use for the text.
		textColor[*it] = isNegative ? QColor(208,0,0) : Qt::black;

		// Text to show in the main field and the bottom field.
		QString mainText ;
		QString extraText;
		if(isNegative) {
			mainText  = _("Flag down");
			extraText = _displayTimeAfterTimeout ? QString(_("Since: %1")).arg(timeDurationAsString(info.total_time)) : "";
		}
		else if(_biTimer->time_control().mode()==TimeControl::Mode::BRONSTEIN && _displayBronsteinExtraInfo) {
			mainText  = timeDurationAsString(info.main_time);
			extraText = info.bronstein_time<=TimeDuration::zero() ? _("Main time") : timeDurationAsString(info.bronstein_time);
		}
		else if(_biTimer->time_control().mode()==TimeControl::Mode::BYO_YOMI && _displayByoYomiExtraInfo) {
			mainText  = timeDurationAsString(info.main_time);
			extraText = info.current_byo_period<=0 ? _("Main time") : (info.total_byo_periods==1 ? _("Byo-yomi period") :
				QString(_("Byo-yomi period %1/%2")).arg(info.current_byo_period).arg(info.total_byo_periods));
		}
		else {
			mainText  = timeDurationAsString(info.total_time);
			extraText = "";
		}

		// Text rendering.
		_painter->setPen(textColor[*it]);
		if(extraText.isEmpty()) {
			drawText(x[*it]+w[*it]*0.1, y+h*0.15, w[*it]*0.8, h*0.7, Qt::AlignCenter, mainText);
		}
		else {
			double        xExtraText    = x[*it] + (*it==Side::LEFT ? 15 : w[*it]*0.4-15);
			Qt::Alignment flagExtraText = Qt::AlignVCenter | (*it==Side::LEFT ? Qt::AlignLeft : Qt::AlignRight);
			drawText(x[*it]+w[*it]*0.1, y+h*0.2    , w[*it]*0.8, h*0.6 , Qt::AlignCenter, mainText );
			drawText(xExtraText       , y+h*0.93-15, w[*it]*0.6, h*0.07, flagExtraText  , extraText);
		}
	}

	// Name rendering.
	if(_showLabels)
	{
		// Compute and apply the font factor to use.
		double fontFactor = std::min(
			computeFontFactor(w[Side::LEFT ]*0.9, h*0.1, _label[Side::LEFT ]),
			computeFontFactor(w[Side::RIGHT]*0.9, h*0.1, _label[Side::RIGHT])
		);
		applyFontFactor(fontFactor);

		// Text rendering.
		for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it) {
			if(_label[*it].isEmpty()) {
				continue;
			}
			_painter->setPen(textColor[*it]);
			_painter->drawText(QRectF(x[*it]+w[*it]*0.05, y+15, w[*it]*0.9, h*0.1), Qt::AlignCenter, _label[*it]);
		}
	}
}


// Draw the string `text` in the rectangle defined by the given coordinates, with the largest possible font.
void BiTimerWidget::drawText(double x, double y, double w, double h, Qt::Alignment flags, const QString &text)
{
	// Compute and apply the font factor to use.
	double factor = computeFontFactor(w, h, text);
	_painter->save();
	applyFontFactor(factor);

	// Draw the text.
	_painter->drawText(QRectF(x, y, w, h), flags, text);

	// Let the painter in its initial state.
	_painter->restore();
}


// Apply the given factor to the current font.
void BiTimerWidget::applyFontFactor(double factor)
{
	QFont font = _painter->font();
	font.setPointSizeF(font.pointSizeF() * factor);
	_painter->setFont(font);
}


// Compute the font factor to apply to fit the string `text` in a rectangle of size `w x h`.
double BiTimerWidget::computeFontFactor(double w, double h, const QString &text) const
{
	QRectF br = _painter->boundingRect(QRectF(0, 0, w, h), Qt::AlignCenter, text);
	double retVal = std::min(w/br.width(), h/br.height());
	return std::floor(retVal*64)/64;
}


// Build the string representation corresponding to a given time duration.
// No minus sign is returned for negative time durations.
QString BiTimerWidget::timeDurationAsString(const TimeDuration &value) const
{
	int total_seconds = std::abs(to_seconds(value));
	int seconds = total_seconds;
	int minutes = seconds/60; seconds = seconds%60;
	int hours   = minutes/60; minutes = minutes%60;
	if(total_seconds<to_seconds(_delayBeforeDisplaySeconds)) {
		return hours>0 ?
			QString("%1:%2.%3").arg(hours).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')) :
			QString("%1.%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
	}
	else {
		return QString("%1:%2").arg(hours).arg(minutes, 2, 10, QChar('0'));
	}
}

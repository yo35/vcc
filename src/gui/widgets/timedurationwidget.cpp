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


#include "timedurationwidget.h"
#include <wrappers/translation.h>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>


// Constructor.
TimeDurationWidget::TimeDurationWidget(QWidget *parent) : QWidget(parent), _shuntSignal(false)
{
	// Create the fields
	_hrs = new QSpinBox(this);
	_min = new QSpinBox(this);
	_sec = new QSpinBox(this);
	_hrs->setRange(0, 99);
	_min->setRange(0, 59);
	_sec->setRange(0, 59);
	connect(_hrs, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &TimeDurationWidget::onFieldChanged);
	connect(_min, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &TimeDurationWidget::onFieldChanged);
	connect(_sec, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &TimeDurationWidget::onFieldChanged);

	// Geometry
	QHBoxLayout *layout = new QHBoxLayout;
	setLayout(layout);
	layout->setMargin(0);
	layout->addWidget(_hrs, 1); layout->addWidget(new QLabel(_("hour"), this), 0, Qt::AlignCenter);
	layout->addWidget(_min, 1); layout->addWidget(new QLabel(_("min" ), this), 0, Qt::AlignCenter);
	layout->addWidget(_sec, 1); layout->addWidget(new QLabel(_("sec" ), this), 0, Qt::AlignCenter);
}


// Value displayed in the widget.
TimeDuration TimeDurationWidget::value() const
{
	return from_seconds(_hrs->value()*3600 + _min->value()*60 + _sec->value());
}


// Change the value displayed in the widget.
void TimeDurationWidget::setValue(const TimeDuration &td)
{
	if(td==value()) {
		return;
	}
	long sec = to_seconds(td);
	_shuntSignal = true;
	if(sec<=0) {
		_sec->setValue(0);
		_min->setValue(0);
		_hrs->setValue(0);
	}
	else {
		_sec->setValue(sec %  60); sec /= 60;
		_min->setValue(sec %  60); sec /= 60;
		_hrs->setValue(sec % 100);
	}
	_shuntSignal = false;
	emit valueChanged();
}


// Handler for the value-changed signal of the spin box fields
void TimeDurationWidget::onFieldChanged()
{
	if(_shuntSignal) {
		return;
	}
	emit valueChanged();
}

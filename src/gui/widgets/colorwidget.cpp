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


#include "colorwidget.h"
#include <QPainter>


// Default constructor.
ColorWidget::ColorWidget(QWidget *parent) : QWidget(parent), _color(Qt::white) {}


// Constructor.
ColorWidget::ColorWidget(QBrush color, QWidget *parent) : QWidget(parent), _color(color) {}


// Size hints.
QSize ColorWidget::minimumSizeHint() const { return QSize(30, 30); }
QSize ColorWidget::sizeHint       () const { return QSize(30, 30); }


// Widget rendering method.
void ColorWidget::paintEvent(QPaintEvent *)
{
	// Create the painter object.
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);

	// Fill the area.
	painter.setBrush(_color);
	painter.drawRoundedRect(0, 0, width(), height(), 5, 5);
}

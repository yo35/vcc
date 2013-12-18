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


#include "keyboardmapwidget.h"
#include <stdexcept>
#include <translation.h>
#include <QPainter>


// Constructor.
KeyboardMapWidget::KeyboardMapWidget(QWidget *parent) :
	QWidget(parent), _keyboardMap(nullptr)
{}


// Throw an exception if no keyboard map is binded to the widget.
void KeyboardMapWidget::ensureKeyboardMapBinded() const
{
	if(_keyboardMap==nullptr) {
		throw std::invalid_argument(_("No keyboard map is currently binded to the widget."));
	}
}


// Bind a keyboard map to the widget.
void KeyboardMapWidget::bindKeyboardMap(const KeyboardMap &keyboardMap)
{
	// Nothing to do if the new keyboard map is identical to the old one.
	if(_keyboardMap==&keyboardMap) {
		return;
	}

	// Otherwise, bind the new keyboard map, and update the widget.
	_keyboardMap = &keyboardMap;
	update();
}


// Un-bind the keyboard map currently binded, if any.
void KeyboardMapWidget::unbindKeyboardMap()
{
	// Nothing to do if no keyboard map is currently binded.
	if(_keyboardMap==nullptr) {
		return;
	}

	// Otherwise, un-bind the old keyboard map, and update the widget.
	_keyboardMap = nullptr;
	update();
}


// Size hints.
QSize KeyboardMapWidget::minimumSizeHint() const { return QSize(500, 200); }
QSize KeyboardMapWidget::sizeHint       () const { return QSize(800, 300); }


// Widget rendering method.
void KeyboardMapWidget::paintEvent(QPaintEvent *)
{
	// Create the painter object.
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);

	// Background
	painter.setBrush(QColor(208, 255, 208));
	painter.drawRect(0, 0, width(), height());
	if(_keyboardMap==nullptr) {
		return;
	}

	//TODO
}

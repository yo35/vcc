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
#include <cassert>
#include <algorithm>
#include <numeric>
#include <translation.h>
#include <QPainter>


// Constructor.
KeyboardMapWidget::KeyboardMapWidget(QWidget *parent) :
	QWidget(parent), _displayNumericKeypad(true), _keyboardMap(nullptr),
	_painter(nullptr), _keyMargin(0), _keyRadius(0)
{}


// Throw an exception if no keyboard map is binded to the widget.
void KeyboardMapWidget::ensureKeyboardMapBinded() const
{
	if(_keyboardMap==nullptr) {
		throw std::invalid_argument(_("No keyboard map is currently binded to the widget."));
	}
}


// Set whether the numeric keypad should be displayed or not.
void KeyboardMapWidget::setDisplayNumericKeypad(bool value)
{
	// Nothing to do if the new value is equal to the old one.
	if(value==_displayNumericKeypad) {
		return;
	}

	// Otherwise, change the value and update the widget.
	_displayNumericKeypad = value;
	update();
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

	// Transform the coordinate system of the painter to ease the rendering.
	// The keyboard is drawn in a bounding box (rectangle) of size
	// _keyboardMap->total_height() * _keyboardMap->total_width()
	// and whose top-left corner corresponds to the point (0,0) in the new coordinate system.
	// It is centered with respect to the full widget area.
	int margin = 5; // minimal distance between the widget borders (l,r,t,b) and the keyboard bounding box.
	int keyboardWidth  = _keyboardMap->total_width(_displayNumericKeypad);
	int keyboardHeight = _keyboardMap->total_height();
	double xScale = static_cast<double>(width () - 2*margin) / static_cast<double>(keyboardWidth );
	double yScale = static_cast<double>(height() - 2*margin) / static_cast<double>(keyboardHeight);
	double scale  = std::min(xScale, yScale);
	int xMargin = (width () - static_cast<int>(scale*keyboardWidth )) / 2;
	int yMargin = (height() - static_cast<int>(scale*keyboardHeight)) / 2;
	painter.translate(QPoint(xMargin, yMargin));
	painter.scale(scale, scale);

	// Prepare the rendering of the keys
	_painter = &painter;
	_keyMargin = 10;
	_keyRadius = 0;

	//painter.setBrush(Qt::red);
	//painter.drawRect(0, 0, keyboardWidth, keyboardHeight);


	//painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::))
	painter.setBrush(Qt::white);
	painter.setBrush(Qt::blue);

	for(std::size_t k=0; k<_keyboardMap->key_count(); ++k) {

		painter.setBrush(_keyboardMap->key(k).line_extent()==1 ? Qt::blue : Qt::red);
		drawKeyShape(k);
	}

	//drawRectangularKeyShape(0, 0, 130, 100);
	//drawRectangularKeyShape(130, 0, 100, 100);
	//drawRectangularKeyShape(0, 100, 250, 100);
	//painter.drawRect(0, 0, 130, 100);


	//TODO
}


// Draw the shape of the key corresponding to the given index.
void KeyboardMapWidget::drawKeyShape(std::size_t idx)
{
	const KeyboardMap::KeyDescriptor &key(_keyboardMap->key(idx));
	std::size_t line = key.line_top();

	// Easy case: the key lies on a single key-line.
	if(key.line_extent()==1) {
		int x = key.x    (line);
		int w = key.width(line);
		int y = _keyboardMap->line_y     (line);
		int h = _keyboardMap->line_height(line);
		drawRectangularKeyShape(x+_keyMargin, y+_keyMargin, w-2*_keyMargin, h-2*_keyMargin);
	}

	// Complex case: the key lies on several key-lines.
	else {
		std::vector<int> dxLeft ; dxLeft .reserve(key.line_extent()-1);
		std::vector<int> dxRight; dxRight.reserve(key.line_extent()-1);
		std::vector<int> dyLeft ; dyLeft .reserve(key.line_extent()  );
		std::vector<int> dyRight; dyRight.reserve(key.line_extent()  );

		// Create a new connected component
		int xLeft  = key.x(line) + _keyMargin;
		int xRight = xLeft + key.width(line) - 2*_keyMargin;
		int x0     = xLeft;
		int dxTop  = xRight - xLeft;
		int y0 = _keyboardMap->line_y(line) + _keyMargin;
		dyLeft .push_back(_keyboardMap->line_height(line) - 2*_keyMargin);
		dyRight.push_back(_keyboardMap->line_height(line) - 2*_keyMargin);

		while(line<key.line_bottom()) {
			++line;
			int newXLeft  = key.x(line) + _keyMargin;
			int newXRight = newXLeft + key.width(line) - 2*_keyMargin;

			// New connected component encountered: render the current CC, and create a new one.
			if(newXRight<=xLeft || newXLeft>=xRight) {
				drawPolygonalKeyShape(x0, y0, dxTop, xRight-xLeft, dxLeft, dyLeft, dxRight, dyRight);
				dxLeft .clear();
				dxRight.clear();
				dyLeft .clear();
				dyRight.clear();
				xLeft  = newXLeft;
				xRight = newXRight;
				x0     = xLeft;
				dxTop  = xRight - xLeft;
				y0 = _keyboardMap->line_y(line) + _keyMargin;
				dyLeft .push_back(_keyboardMap->line_height(line) - 2*_keyMargin);
				dyRight.push_back(_keyboardMap->line_height(line) - 2*_keyMargin);
			}

			// Regular scenario: include the rectangle on the current line to the current connected component.
			else
			{
				// Update/add a left edge
				if(newXLeft==xLeft) {
					dyLeft.back() += _keyboardMap->line_height(line);
				}
				else {
					if(newXLeft<xLeft) {
						dyLeft.back() += 2*_keyMargin;
						dyLeft.push_back(_keyboardMap->line_height(line) - 2*_keyMargin);
					}
					else {
						dyLeft.push_back(_keyboardMap->line_height(line));
					}
					dxLeft.push_back(newXLeft - xLeft);
					xLeft = newXLeft;
				}

				// Update/add a right edge
				if(newXRight==xRight) {
					dyRight.back() += _keyboardMap->line_height(line);
				}
				else {
					if(newXRight>xRight) {
						dyRight.back() += 2*_keyMargin;
						dyRight.push_back(_keyboardMap->line_height(line) - 2*_keyMargin);
					}
					else {
						dyRight.push_back(_keyboardMap->line_height(line));
					}
					dxRight.push_back(newXRight - xRight);
					xRight = newXRight;
				}
			}
		}

		// Render the last connected component
		drawPolygonalKeyShape(x0, y0, dxTop, xRight-xLeft, dxLeft, dyLeft, dxRight, dyRight);
	}
}


// Draw the shape of a rectangular key (i.e. a key that lies on a single key-line).
void KeyboardMapWidget::drawRectangularKeyShape(int x, int y, int w, int h)
{
	int radius = std::min(_keyRadius, std::min(w,h)/2);
	_painter->drawRoundedRect(x, y, w, h, radius, radius);
}


// Draw the shape of a polygonal key (i.e. a key that lies on several key-lines).
void KeyboardMapWidget::drawPolygonalKeyShape(int x0, int y0, int dxTop, int dxBottom,
	const std::vector<int> &dxLeft , const std::vector<int> &dyLeft ,
	const std::vector<int> &dxRight, const std::vector<int> &dyRight)
{
	assert(dxLeft .size()+1==dyLeft .size());
	assert(dxRight.size()+1==dxRight.size());
	assert(std::accumulate(dyLeft.begin(),dyLeft.end())==std::accumulate(dyRight.begin(),dyRight.end()));
	assert(std::accumulate(dxLeft.begin(),dxLeft.end())+dxBottom==dxTop+std::accumulate(dyLeft.begin(),dyLeft.end()));

	// Initialize the path
	int x = x0;
	int y = y0;
	QPainterPath path(QPoint(x,y));

	// Left edges
	y += dyLeft[0]; path.lineTo(x,y);
	for(std::size_t k=0; k<dxLeft.size(); ++k) {
		x += dxLeft[k  ]; path.lineTo(x,y);
		y += dyLeft[k+1]; path.lineTo(x,y);
	}

	// Bottom edge
	x += dxBottom; path.lineTo(x,y);

	// Right edges
	for(std::size_t k=dxRight.size(); k>=1; --k) {
		y -= dyRight[k  ]; path.lineTo(x,y);
		x -= dxRight[k-1]; path.lineTo(x,y);
	}
	y -= dyRight[0]; path.lineTo(x,y);

	// Top edge
	x -= dxTop;
	path.closeSubpath();

	// Fill the path
	_painter->fillPath(path, _painter->brush());
}

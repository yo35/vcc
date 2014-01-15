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


#include "keyboardwidget.h"
#include "keyboardhandler.h"
#include "keyboardmap.h"
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <translation.h>
#include <QPainter>


// Constructor.
KeyboardWidget::KeyboardWidget(const KeyboardHandler *keyboardHandler, QWidget *parent) :
	QWidget(parent),
	_keyboardHandler(keyboardHandler), _keyboardMap(nullptr), _shortcutMap(nullptr),
	_hasNumericKeypad(true), _showShortcutHigh(false), _modifierKeys(ModifierKeys::DOUBLE_CTRL),
	_colorBackground (208, 255, 208),
	_colorKeyDefault (Qt::white),
	_colorKeyDown    (0, 0, 128),
	_colorModifierKey(Qt::white),
	_painter(nullptr), _keyMargin(0), _keyRadius(0)
{
	connect(_keyboardHandler, &KeyboardHandler::keyPressed , this, &KeyboardWidget::onKeyStateChanged);
	connect(_keyboardHandler, &KeyboardHandler::keyReleased, this, &KeyboardWidget::onKeyStateChanged);
}


// Throw an exception if no keyboard map is binded to the widget.
void KeyboardWidget::ensureKeyboardMapBinded() const
{
	if(_keyboardMap==nullptr) {
		throw std::invalid_argument(_("No keyboard map is currently binded to the widget."));
	}
}


// Throw an exception if no shortcut map is binded to the widget.
void KeyboardWidget::ensureShortcutMapBinded() const
{
	if(_shortcutMap==nullptr) {
		throw std::invalid_argument(_("No shortcut map is currently binded to the widget."));
	}
}


// Set whether the numeric keypad should be displayed or not.
void KeyboardWidget::setHasNumericKeypad(bool value)
{
	if(value==_hasNumericKeypad) {
		return;
	}
	_hasNumericKeypad = value;
	update();
}


// Set whether the high-position shortcuts are represented.
void KeyboardWidget::setShowShortcutHigh(bool value)
{
	if(value==_showShortcutHigh) {
		return;
	}
	_showShortcutHigh = value;
	update();
}


// Change the modifier keys.
void KeyboardWidget::setModifierKeys(ModifierKeys value)
{
	if(value==_modifierKeys) {
		return;
	}
	_modifierKeys = value;
	update();
}


// Bind a keyboard map to the widget.
void KeyboardWidget::bindKeyboardMap(const KeyboardMap &keyboardMap)
{
	if(_keyboardMap==&keyboardMap) {
		return;
	}
	_keyboardMap = &keyboardMap;
	update();
}


// Un-bind the keyboard map currently binded, if any.
void KeyboardWidget::unbindKeyboardMap()
{
	if(_keyboardMap==nullptr) {
		return;
	}
	_keyboardMap = nullptr;
	update();
}


// Bind a shortcut map to the widget.
void KeyboardWidget::bindShortcutMap(const ShortcutMap &shortcutMap)
{
	if(_shortcutMap==&shortcutMap) {
		return;
	}
	_shortcutMap = &shortcutMap;
	update();
}


// Un-bind the shortcut map currently binded, if any.
void KeyboardWidget::unbindShortcutMap()
{
	if(_shortcutMap==nullptr) {
		return;
	}
	_shortcutMap = nullptr;
	update();
}


// Size hints.
QSize KeyboardWidget::minimumSizeHint() const { return QSize(500, 200); }
QSize KeyboardWidget::sizeHint       () const { return QSize(800, 300); }


// Called when a key is pressed or released.
void KeyboardWidget::onKeyStateChanged(std::uint32_t)
{
	update();
}


// Widget rendering method.
void KeyboardWidget::paintEvent(QPaintEvent *)
{
	// Create the painter object.
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(Qt::NoPen);

	// Background
	painter.setBrush(_colorBackground);
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
	int keyboardWidth  = _keyboardMap->total_width(_hasNumericKeypad);
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
	_keyMargin = std::min(5, KeyboardMap::MINIMUM_KEY_SIZE/2);
	_keyRadius = 15;
	painter.setBrush(_colorKeyDefault);

	// Draw the keys
	for(std::size_t k=0; k<_keyboardMap->key_count(); ++k)
	{
		const KeyboardMap::KeyDescriptor &key(_keyboardMap->key(k));
		bool isKeyDown = _keyboardHandler->isDown(key.scan_code());

		// Determine the color to use to draw the key
		painter.save();
		if(isKeyDown) {
			painter.setBrush(_colorKeyDown);
		}
		else if(isModifierKey(key)) {
			painter.setBrush(_colorModifierKey);
		}
		else if(_shortcutMap!=nullptr) {
			int shortcut = _shortcutMap->shortcut(key.id(), _showShortcutHigh);
			if(shortcut>0) {
				auto it = _shortcutColors.find(shortcut);
				if(it!=_shortcutColors.end()) {
					painter.setBrush(it->second);
				}
			}
		}

		// Draw the key
		drawKeyShape(key);
		painter.setPen(isKeyDown ? Qt::white : Qt::black);
		drawKeyLabel(key);
		painter.restore();
	}
}


// Whether the given key is a modifier key.
bool KeyboardWidget::isModifierKey(const KeyboardMap::KeyDescriptor &key) const
{
	switch(_modifierKeys) {
		case ModifierKeys::DOUBLE_CTRL : return key.id()=="ctrl-left"  || key.id()=="ctrl-right" ;
		case ModifierKeys::DOUBLE_SHIFT: return key.id()=="shift-left" || key.id()=="shift-right";
		case ModifierKeys::DOUBLE_ALT  : return key.id()=="alt-left"   || key.id()=="alt-right"  ;
		default: return false;
	}
}


// Draw the label of the key corresponding to the given index.
void KeyboardWidget::drawKeyLabel(const KeyboardMap::KeyDescriptor &key)
{
	std::size_t line = key.line_top();

	// The label is drawn on the key-line corresponding to the larger area of the key.
	//  -> Search this key-line.
	for(std::size_t k=line+1; k<key.line_bottom(); ++k) {
		if(key.width(k)>key.width(line)) {
			line = k;
		}
	}

	// Convert the label fields into QStrings
	std::vector<QString> field(key.label().size());
	std::transform(key.label().begin(), key.label().end(), field.begin(), &QString::fromStdString);

	// Draw the label (that may be composed of several fields) in a rectangle
	// of size (w,h) located at (x,y).
	double x = key.x    (line) + _keyMargin;
	double w = key.width(line) - 2*_keyMargin;
	double y = _keyboardMap->line_y     (line) + _keyMargin;
	double h = _keyboardMap->line_height(line) - 2*_keyMargin;

	// Actual available size for each field, and remaining spaces
	double aw = field.size()<=2 ? w*0.8 : (w*0.9)/2;
	double ah = field.size()<=1 ? h*0.6 : (h*0.9)/2;
	double rw = w - aw*(field.size()<=2 ? 1 : 2);
	double rh = h - ah*(field.size()<=1 ? 1 : 2);

	// Re-scale the font
	double factor = computeFontFactor(aw, ah, field);
	QFont font = _painter->font();
	font.setPointSizeF(font.pointSizeF() * factor);
	_painter->setFont(font);

	// Draw the fields
	if(field.size()==1) {
		_painter->drawText(QRectF(x+rw/2, y+rh/2, aw, ah), Qt::AlignCenter, field[0]);
	}
	else if(field.size()==2) {
		_painter->drawText(QRectF(x+rw/2, y   +rh/2, aw, ah), Qt::AlignCenter, field[1]);
		_painter->drawText(QRectF(x+rw/2, y+ah+rh/2, aw, ah), Qt::AlignCenter, field[0]);
	}
	else if(field.size()==3 || field.size()==4) {
		_painter->drawText(QRectF(x   +rw/2, y   +rh/2, aw, ah), Qt::AlignCenter, field[1]);
		_painter->drawText(QRectF(x   +rw/2, y+ah+rh/2, aw, ah), Qt::AlignCenter, field[0]);
		_painter->drawText(QRectF(x+aw+rw/2, y+ah+rh/2, aw, ah), Qt::AlignCenter, field[2]);
		if(field.size()==4) {
			_painter->drawText(QRectF(x+aw+rw/2, y   +rh/2, aw, ah), Qt::AlignCenter, field[3]);
		}
	}
}


// Draw the shape of the key corresponding to the given index.
void KeyboardWidget::drawKeyShape(const KeyboardMap::KeyDescriptor &key)
{
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
void KeyboardWidget::drawRectangularKeyShape(int x, int y, int w, int h)
{
	int radius = std::min(_keyRadius, std::min(w,h)/2);
	_painter->drawRoundedRect(x, y, w, h, radius, radius);
}


// Draw the shape of a polygonal key (i.e. a key that lies on several key-lines).
void KeyboardWidget::drawPolygonalKeyShape(int x0, int y0, int dxTop, int dxBottom,
	const std::vector<int> &dxLeft , const std::vector<int> &dyLeft ,
	const std::vector<int> &dxRight, const std::vector<int> &dyRight)
{
	assert(dxLeft .size()+1==dyLeft .size());
	assert(dxRight.size()+1==dyRight.size());
	assert(std::accumulate(dyLeft.begin(),dyLeft.end(),0)         ==std::accumulate(dyRight.begin(),dyRight.end(),0)      );
	assert(std::accumulate(dxLeft.begin(),dxLeft.end(),0)+dxBottom==std::accumulate(dxRight.begin(),dxRight.end(),0)+dxTop);

	// Determine the radius of the arcs to apply to the corners
	std::vector<int> radiusLeft (computeCornerRadius(dxTop, dxBottom, dxLeft , dyLeft ));
	std::vector<int> radiusRight(computeCornerRadius(dxTop, dxBottom, dxRight, dyRight));

	// Initialize the path
	int x = x0;
	int y = y0;
	std::size_t k = 0;
	QPainterPath path(QPoint(x+radiusLeft[0], y));

	// Left edges
	while(true) {
		// 0 <= k < dyLeft.size()
		path.quadTo(QPoint(x, y), QPoint(x, y+radiusLeft[2*k]));
		y += dyLeft[k]; path.lineTo(x, y-radiusLeft[2*k+1]);
		if(k>=dxLeft.size()) {
			break;
		}
		// 0 <= k < dxLeft.size()
		path.quadTo(QPoint(x, y), QPoint(x+(dxLeft[k]>0 ? 1 : -1)*radiusLeft[2*k+1], y));
		x += dxLeft[k]; path.lineTo(x-(dxLeft[k]>0 ? 1 : -1)*radiusLeft[2*k+2], y);
		++k;
	}

	// Bottom edge
	path.quadTo(QPoint(x, y), QPoint(x+radiusLeft[2*k], y));
	k = dxRight.size();
	x += dxBottom; path.lineTo(x-radiusRight[2*k+1], y);

	// Right edges
	while(true) {
		// 0 <= k < dyRight.size()
		path.quadTo(QPoint(x, y), QPoint(x, y-radiusRight[2*k+1]));
		y -= dyRight[k]; path.lineTo(x, y+radiusRight[2*k]);
		if(k==0) {
			break;
		}
		// 0 <= k-1 < dxRight.size()
		path.quadTo(QPoint(x, y), QPoint(x-(dxRight[k-1]>0 ? 1 : -1)*radiusRight[2*k], y));
		x -= dxRight[k-1]; path.lineTo(x+(dxRight[k-1]>0 ? 1 : -1)*radiusRight[2*k-1], y);
		--k;
	}

	// Top edge
	path.quadTo(QPoint(x, y), QPoint(x-radiusRight[0], y));
	x -= dxTop;
	path.closeSubpath();

	// Fill the path
	_painter->fillPath  (path, _painter->brush());
	_painter->strokePath(path, _painter->pen  ());
}


// Determine the radius of the arcs to apply to the corners when drawing
// a polygonal key shape.
std::vector<int> KeyboardWidget::computeCornerRadius(int dxTop, int dxBottom,
	const std::vector<int> &dx, const std::vector<int> &dy) const
{
	std::size_t n = dy.size();
	assert(dx.size()+1==n);
	std::vector<int> retval(2*n);
	for(std::size_t k=0; k<n; ++k) {
		retval[2*k  ] = std::min(_keyRadius, std::min(std::abs(k==0   ? dxTop    : dx[k-1]), dy[k]) / 2);
		retval[2*k+1] = std::min(_keyRadius, std::min(std::abs(k==n-1 ? dxBottom : dx[k  ]), dy[k]) / 2);
	}
	return std::move(retval);
}


// Determine the factor to apply to the current to make each string in `texts`
// fit into a rectangle of size (w,h).
double KeyboardWidget::computeFontFactor(double w, double h, const std::vector<QString> &texts) const
{
	double retval = DBL_MAX;
	for(const auto &text : texts) {
		QRectF br = _painter->boundingRect(QRectF(0, 0, w, h), Qt::AlignCenter, text);
		retval = std::min(retval, std::min(w/br.width(), h/br.height()));
	}
	return std::round(retval*4)/4;
}

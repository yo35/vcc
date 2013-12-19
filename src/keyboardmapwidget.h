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


#ifndef KEYBOARDMAPWIDGET_H_
#define KEYBOARDMAPWIDGET_H_

#include <QWidget>
#include <vector>
#include <set>
#include "keyboardmap.h"
QT_BEGIN_NAMESPACE
	class QPainter;
QT_END_NAMESPACE


/**
 * Display a keyboard map and edit the associated key association map.
 */
class KeyboardMapWidget : public QWidget
{
	Q_OBJECT

public:

	/**
	 * Constructor
	 */
	KeyboardMapWidget(QWidget *parent=0);

	/**
	 * Whether the numeric keypad should be displayed or not.
	 */
	bool displayNumericKeypad() const { return _displayNumericKeypad; }

	/**
	 * Set whether the numeric keypad should be displayed or not.
	 */
	void setDisplayNumericKeypad(bool value);

	/**
	 * Check whether a keyboard map is binded to the widget or not.
	 */
	bool hasKeyboardMapBinded() const { return _keyboardMap!=nullptr; }

	/**
	 * Return a reference to the binded keyboard map object.
	 * @throw std::invalid_argument If no keyboard map is binded to the widget.
	 */
	const KeyboardMap &keyboardMap() const { ensureKeyboardMapBinded(); return *_keyboardMap; }

	/**
	 * Bind a keyboard map to the widget.
	 */
	void bindKeyboardMap(const KeyboardMap &keyboardMap);

	/**
	 * Unbind the keyboard map currently binded, if any.
	 */
	void unbindKeyboardMap();

	/**
	 * @name Size hint methods.
	 * @{
	 */
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	/**@} */

protected:

	/**
	 * @name Key event handlers.
	 * @{
	 */
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	/**@} */

	/**
	 * Widget rendering method.
	 */
	void paintEvent(QPaintEvent *event) override;

private:

	// Private functions
	void ensureKeyboardMapBinded() const;
	void drawKeyLabel(std::size_t idx);
	void drawKeyShape(std::size_t idx);
	void drawRectangularKeyShape(int x, int y, int w, int h);
	void drawPolygonalKeyShape(int x0, int y0, int dxTop, int dxBottom,
		const std::vector<int> &dxLeft , const std::vector<int> &dyLeft ,
		const std::vector<int> &dxRight, const std::vector<int> &dyRight);
	std::vector<int> computeCornerRadius(int dxTop, int dxBottom,
		const std::vector<int> &dx, const std::vector<int> &dy) const;
	double computeFontFactor(double w, double h, const std::vector<QString> &texts) const;

	// Private members
	bool                    _displayNumericKeypad;
	const KeyboardMap      *_keyboardMap         ;
	std::set<std::uint32_t> _keyDown             ;

	// Colors
	QColor _colorBackground;
	QColor _colorKeyDefault;
	QColor _colorKeyDown   ;

	// Temporary members used at rendering time
	// (should not be used outside the `paintEvent` method).
	QPainter *_painter  ;
	int       _keyMargin;
	int       _keyRadius;
};

#endif /* KEYBOARDMAPWIDGET_H_ */

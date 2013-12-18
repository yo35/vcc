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
#include "keyboardmap.h"


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
	 * Widget rendering method.
	 */
	void paintEvent(QPaintEvent *event) override;

private:

	// Private functions
	void ensureKeyboardMapBinded() const;

	// Private members
	const KeyboardMap *_keyboardMap;
};

#endif /* KEYBOARDMAPWIDGET_H_ */

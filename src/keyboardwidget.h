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


#ifndef KEYBOARDWIDGET_H_
#define KEYBOARDWIDGET_H_

#include <QWidget>
#include <vector>
#include <map>
#include <memory>
#include <boost/optional.hpp>
QT_BEGIN_NAMESPACE
	class QPainter;
QT_END_NAMESPACE
#include "keys.h"
#include "keyboardhandler.h"
#include "keyboardmap.h"
#include "shortcutmap.h"


/**
 * Display a keyboard map and edit the associated shortcut map.
 */
class KeyboardWidget : public QWidget
{
	Q_OBJECT

public:

	/**
	 * Constructor
	 */
	KeyboardWidget(const KeyboardHandler *keyboardHandler, QWidget *parent=0);

	/**
	 * Colors to use for the keys that are associated to a shortcut.
	 */
	std::map<int, QColor> &shortcutColors() { return _shortcutColors; }

	/**
	 * Colors to use for the keys that are associated to a shortcut (constant version).
	 */
	const std::map<int, QColor> &shortcutColors() const { return _shortcutColors; }

	/**
	 * Modifier key brush.
	 */
	QBrush modifierKeyBrush() const { return _modifierKeyBrush; }

	/**
	 * Change the modifier key brush.
	 */
	void setModifierKeyBrush(QBrush brush) { _modifierKeyBrush = brush; }

	/**
	 * Whether the numeric keypad should be displayed or not.
	 */
	bool hasNumericKeypad() const { return _hasNumericKeypad; }

	/**
	 * Set whether the numeric keypad should be displayed or not.
	 */
	void setHasNumericKeypad(bool value);

	/**
	 * Whether the high-position shortcuts are represented.
	 */
	bool showShortcutHigh() const { return _showShortcutHigh; }

	/**
	 * Set whether the high-position shortcuts are represented.
	 */
	void setShowShortcutHigh(bool value);

	/**
	 * Modifier keys.
	 */
	ModifierKeys modifierKeys() const { return _modifierKeys; }

	/**
	 * Change the modifier keys.
	 */
	void setModifierKeys(ModifierKeys value);

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
	 * Un-bind the keyboard map currently binded, if any.
	 */
	void unbindKeyboardMap();

	/**
	 * Check whether a shortcut map is binded to the widget or not.
	 */
	bool hasShortcutMapBinded() const { return _shortcutMap!=nullptr; }

	/**
	 * Return a reference to the binded shortcut map object.
	 * @throw std::invalid_argument If no shortcut map is binded to the widget.
	 */
	const ShortcutMap &shortcutMap() const { ensureShortcutMapBinded(); return *_shortcutMap; }

	/**
	 * Bind a shortcut map to the widget.
	 */
	void bindShortcutMap(const ShortcutMap &shortcutMap);

	/**
	 * Un-bind the shortcut map currently binded, if any.
	 */
	void unbindShortcutMap();

	/**
	 * @name Size hint methods.
	 * @{
	 */
	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;
	/**@} */

signals:

	/**
	 * Signal triggered when the user clicks on the graphic representation of a key.
	 */
	void keyClicked(const std::string &id, Qt::MouseButton button);

protected:

	/**
	 * Mouse-press handler.
	 */
	void mousePressEvent(QMouseEvent *event) override;

	/**
	 * Widget rendering method.
	 */
	void paintEvent(QPaintEvent *event) override;

private:

	// Private functions
	void ensureKeyboardMapBinded() const;
	void ensureShortcutMapBinded() const;
	void onKeyStateChanged(ScanCode scanCode);
	bool isModifierKey(const KeyboardMap::KeyDescriptor &key) const;
	void drawKeyLabel(const KeyboardMap::KeyDescriptor &key);
	void drawKeyShape(const KeyboardMap::KeyDescriptor &key);
	void drawRectangularKeyShape(int x, int y, int w, int h);
	void drawPolygonalKeyShape(int x0, int y0, int dxTop, int dxBottom,
		const std::vector<int> &dxLeft , const std::vector<int> &dyLeft ,
		const std::vector<int> &dxRight, const std::vector<int> &dyRight);
	std::vector<int> computeCornerRadius(int dxTop, int dxBottom,
		const std::vector<int> &dx, const std::vector<int> &dy) const;
	double computeFontFactor(double w, double h, const std::vector<QString> &texts) const;

	// Private members
	const KeyboardHandler                  *_keyboardHandler      ;
	const KeyboardMap                      *_keyboardMap          ;
	const ShortcutMap                      *_shortcutMap          ;
	std::unique_ptr<sig::scoped_connection> _shortcutMapConnection;
	bool                                    _hasNumericKeypad     ;
	bool                                    _showShortcutHigh     ;
	ModifierKeys                            _modifierKeys         ;

	// Colors
	QColor _colorBackground ;
	QColor _colorKeyDefault ;
	QColor _colorKeyDown    ;
	QBrush _modifierKeyBrush;
	std::map<int, QColor> _shortcutColors;

	// Temporary members used at rendering time
	QPainter *_painter  ; // invalid used outside the `paintEvent` method
	double    _scale    ;
	int       _xMargin  ;
	int       _yMargin  ;
	int       _keyMargin;
	int       _keyRadius;
};

#endif /* KEYBOARDWIDGET_H_ */

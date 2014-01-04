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


#ifndef COLORWIDGET_H_
#define COLORWIDGET_H_

#include <QWidget>


/**
 * Draw a square with a tunable color.
 */
class ColorWidget : public QWidget
{
	Q_OBJECT

public:

	/**
	 * Default constructor.
	 */
	ColorWidget(QWidget *parent=0);

	/**
	 * Constructor.
	 */
	ColorWidget(QBrush color, QWidget *parent=0);

	/**
	 * Return the selected color.
	 */
	QBrush color() const { return _color; }

	/**
	 * Change the selected color.
	 */
	void set_color(QBrush color) { _color=color; update(); }

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

	// Private members
	QBrush _color;
};

#endif /* COLORWIDGET_H_ */

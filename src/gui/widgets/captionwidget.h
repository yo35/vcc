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


#ifndef CAPTIONWIDGET_H_
#define CAPTIONWIDGET_H_

#include <QWidget>
#include <QToolButton>
#include "colorwidget.h"


/**
 * A colored box + a text to explain what the color is supposed to represent.
 */
class CaptionWidget : public QWidget
{
	Q_OBJECT

public:

	/**
	 * Default constructor.
	 */
	CaptionWidget(QWidget *parent=0);

	/**
	 * Constructor.
	 */
	CaptionWidget(QBrush color, const QString &text, QWidget *parent=0);

	/**
	 * Return the selected color.
	 */
	QBrush color() const { return _caption->color(); }

	/**
	 * Change the selected color.
	 */
	void setColor(QBrush color) { _caption->setColor(color); }

	/**
	 * Return the text of the label.
	 */
	QString text() const { return _label->text(); }

	/**
	 * Set the text of the label.
	 */
	void setText(const QString &text) { _label->setText(text); }

	/**
	 * Return the underlying button (constant version).
	 */
	const QToolButton *button() const { return _label; }

	/**
	 * Return the underlying button.
	 */
	QToolButton *button() { return _label; }

private:

	// Private function
	void buildLayout();

	// Private members
	ColorWidget *_caption;
	QToolButton *_label  ;
};

#endif /* CAPTIONWIDGET_H_ */

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


#include "captionwidget.h"
#include <QHBoxLayout>


// Default constructor.
CaptionWidget::CaptionWidget(QWidget *parent) : QWidget(parent),
	_caption(new ColorWidget(this)), _label(new QLabel(this))
{
	buildLayout();
}


// Constructor.
CaptionWidget::CaptionWidget(QBrush color, const QString &text, QWidget *parent) :
	QWidget(parent),
	_caption(new ColorWidget(color, this)), _label(new QLabel(text, this))
{
	buildLayout();
}


// Set-up the widget layout.
void CaptionWidget::buildLayout()
{
	QHBoxLayout *layout = new QHBoxLayout;
	setLayout(layout);
	layout->setMargin(0);
	layout->addWidget(_caption);
	layout->addWidget(_label, 1);
}

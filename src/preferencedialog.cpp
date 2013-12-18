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


#include "preferencedialog.h"
#include "params.h"
#include "keyboardmapwidget.h"
#include <translation.h>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QVBoxLayout>


// Constructor.
PreferenceDialog::PreferenceDialog(QWidget *parent) : QDialog(parent)
{
	// Top-level layout
	setWindowTitle(_("Preferences"));
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	// Tabs
	QTabWidget *tabs = new QTabWidget(this);
	mainLayout->addWidget(tabs, 1);
	tabs->addTab(createKeyboardPage(), _("Keyboard set-up"));
	tabs->addTab(createDisplayPage (), _("Display options"));

	// Validation buttons
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	mainLayout->addWidget(buttons);
}


// Create the keyboard page.
QWidget *PreferenceDialog::createKeyboardPage()
{
	// Page widget
	QWidget *page = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout;
	page->setLayout(layout);

	// Display numeric-keypad check-box
	_displayNumericKeypad = new QCheckBox(_("Display the numeric keypad"), this);
	connect(_displayNumericKeypad, &QCheckBox::toggled, this, &PreferenceDialog::onDisplayNumericKeypadToggled);
	layout->addWidget(_displayNumericKeypad);

	// Keyboard map widget
	_keyboardMapWidget = new KeyboardMapWidget(this);
	_keyboardMapWidget->setDisplayNumericKeypad(_displayNumericKeypad->isChecked());
	_keyboardMapWidget->bindKeyboardMap(Params::get().keyboard_map("FR"));
	layout->addWidget(_keyboardMapWidget, 1);

	// Return the page widget
	return page;
}


// Create the display page.
QWidget *PreferenceDialog::createDisplayPage()
{
	// Page widget
	QWidget *page = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout;
	page->setLayout(layout);

	// Check-boxes
	_displayStatusBar          = new QCheckBox(_("Display the status bar at the bottom of the main window"      ), this);
	_displayTimeAfterFlagDown  = new QCheckBox(_("Display an increasing time counter when the flag is down"     ), this);
	_displayBronsteinExtraTime = new QCheckBox(_("Display extra time information when playing in Bronstein mode"), this);
	_displayByoYomiExtraTime   = new QCheckBox(_("Display extra time information when playing in byo-yomi mode" ), this);
	layout->addWidget(_displayStatusBar         );
	layout->addWidget(_displayTimeAfterFlagDown );
	layout->addWidget(_displayBronsteinExtraTime);
	layout->addWidget(_displayByoYomiExtraTime  );

	// Return the page widget
	layout->addStretch(1);
	return page;
}


// Action performed when the state of the display-numeric-keypad checkbox changes.
void PreferenceDialog::onDisplayNumericKeypadToggled()
{
	_keyboardMapWidget->setDisplayNumericKeypad(_displayNumericKeypad->isChecked());
}


// Load the dialog with the parameters saved in the Params singleton object.
void PreferenceDialog::loadParameters()
{
	// Keyboard page
	_displayNumericKeypad->setChecked(Params::get().show_numeric_keypad());

	// Display page
	_displayStatusBar->setChecked(Params::get().show_status_bar());
	///TODO
}


// Save the new parameters defined from the dialog in the Params singleton object.
void PreferenceDialog::saveParameters()
{
	// Keyboard page
	Params::get().set_show_numeric_keypad(_displayNumericKeypad->isChecked());

	// Display page
	Params::get().set_show_status_bar(_displayStatusBar->isChecked());
	///TODO
}

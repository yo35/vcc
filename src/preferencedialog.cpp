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
#include <translation.h>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QEvent>


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

	// Keyboard selector (combo-box)
	_keyboardSelector = new QComboBox(this);
	feedKeyboardSelector();
	connect(_keyboardSelector, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		this, &PreferenceDialog::onSelectedKeyboardChanged);
	QHBoxLayout *keyboardSelectorLayout = new QHBoxLayout;
	keyboardSelectorLayout->addWidget(new QLabel(_("Select the keyboard layout you are using")));
	keyboardSelectorLayout->addWidget(_keyboardSelector, 1);
	layout->addLayout(keyboardSelectorLayout);

	// Has numeric-keypad check-box
	_hasNumericKeypad = new QCheckBox(_("The keyboard has a numeric keypad"), this);
	connect(_hasNumericKeypad, &QCheckBox::toggled, this, &PreferenceDialog::onHasNumericKeypadToggled);
	layout->addWidget(_hasNumericKeypad);

	// Keyboard handler and keyboard widget
	_keyboardHandler = new KeyboardHandler(this);
	_keyboardWidget = new KeyboardWidget(_keyboardHandler, this);
	onHasNumericKeypadToggled();
	_keyboardWidget->bindShortcutMap(Params::get().shortcut_map("FR")); //TODO
	layout->addWidget(_keyboardWidget, 1);

	// Captions (exception modifier keys)
	_captionLeft   = new CaptionWidget(QColor(  0,176,  0), _("Left player's button" ), this);
	_captionRight  = new CaptionWidget(QColor(  0,128,255), _("Right player's button"), this);
	_captionPause  = new CaptionWidget(QColor(255,128,  0), _("Pause"                ), this);
	_captionReset  = new CaptionWidget(QColor(240, 48,255), _("Reset"                ), this);
	_captionSwitch = new CaptionWidget(QColor(255, 16, 16), _("Switch"               ), this);
	_keyboardWidget->shortcutColors()[1] = _captionLeft  ->color();
	_keyboardWidget->shortcutColors()[2] = _captionRight ->color();
	_keyboardWidget->shortcutColors()[3] = _captionPause ->color();
	_keyboardWidget->shortcutColors()[4] = _captionReset ->color();
	_keyboardWidget->shortcutColors()[5] = _captionSwitch->color();
	QGridLayout *captionLayout = new QGridLayout;
	captionLayout->addWidget(_captionLeft  , 0, 0);
	captionLayout->addWidget(_captionRight , 1, 0);
	captionLayout->addWidget(_captionPause , 0, 1);
	captionLayout->addWidget(_captionReset , 1, 1);
	captionLayout->addWidget(_captionSwitch, 0, 2);

	// Modifier keys selector
	_modifierKeysSelector = new ModifierKeysWidget(this);
	_keyboardWidget->setModifierKeyColor(_modifierKeysSelector->color());
	onModifierKeysChanged();
	connect(_modifierKeysSelector, &ModifierKeysWidget::valueChanged, this, &PreferenceDialog::onModifierKeysChanged);
	captionLayout->addWidget(_modifierKeysSelector, 1, 2);

	// Modifier keys toggle switch
	_modifierKeysToggle = new QPushButton(this);
	_modifierKeysToggle->setCheckable(true);
	_modifierKeysToggle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	onModifierKeysToggled();
	connect(_modifierKeysToggle, &QPushButton::toggled, this, &PreferenceDialog::onModifierKeysToggled);

	// Layout for the caption and the modifier keys toggle
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addLayout(captionLayout, 5);
	bottomLayout->addWidget(_modifierKeysToggle, 2);
	layout->addLayout(bottomLayout);

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


// Window state-change handler.
void PreferenceDialog::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::ActivationChange) {
		_keyboardHandler->setEnabled(isActiveWindow());
	}
	QDialog::changeEvent(event);
}


// Feed the keyboard selector combo-box.
void PreferenceDialog::feedKeyboardSelector()
{
	for(const auto &it : Params::get().keyboards()) {
		_keyboardSelector->addItem(
			Params::get().keyboard_icon(it),
			QString::fromStdString(Params::get().keyboard_name(it)),
			QVariant(it.c_str())
		);
	}
	_keyboardSelector->setCurrentIndex(-1);
}


// Read the index of the currently selected keyboard.
std::string PreferenceDialog::retrieveSelectedKeyboard() const
{
	return _keyboardSelector->itemData(_keyboardSelector->currentIndex()).toString().toStdString();
}


// Action performed when the selected item in the keyboard selector combo-box changes.
void PreferenceDialog::onSelectedKeyboardChanged()
{
	_keyboardWidget->bindKeyboardMap(Params::get().keyboard_map(retrieveSelectedKeyboard()));
}


// Action performed when the state of the has-numeric-keypad check-box changes.
void PreferenceDialog::onHasNumericKeypadToggled()
{
	_keyboardWidget->setHasNumericKeypad(_hasNumericKeypad->isChecked());
}


// Action performed when the modifier keys changes.
void PreferenceDialog::onModifierKeysChanged()
{
	_keyboardWidget->setModifierKeys(_modifierKeysSelector->modifierKeys());
}


// Action performed when the state of the modifier keys button is toggled.
void PreferenceDialog::onModifierKeysToggled()
{
	bool modiferKeysPressed = _modifierKeysToggle->isChecked();
	_modifierKeysToggle->setText(modiferKeysPressed ?
		_("Key functions\nwhen the modifier keys\nare pressed") :
		_("Default\nkey functions")
	);
	_keyboardWidget->setShowShortcutHigh(modiferKeysPressed);
}


// Load the dialog with the parameters saved in the Params singleton object.
void PreferenceDialog::loadParameters()
{
	// Keyboard page
	_keyboardSelector->setCurrentIndex(_keyboardSelector->findData(QVariant(Params::get().current_keyboard().c_str())));
	_modifierKeysSelector->setModifierKeys(Params::get().modifier_keys());
	_hasNumericKeypad->setChecked(Params::get().has_numeric_keypad());

	// Display page
	_displayStatusBar->setChecked(Params::get().show_status_bar());
	///TODO
}


// Save the new parameters defined from the dialog in the Params singleton object.
void PreferenceDialog::saveParameters()
{
	// Keyboard page
	Params::get().set_current_keyboard(retrieveSelectedKeyboard());
	Params::get().set_modifier_keys(_modifierKeysSelector->modifierKeys());
	Params::get().set_has_numeric_keypad(_hasNumericKeypad->isChecked());

	// Display page
	Params::get().set_show_status_bar(_displayStatusBar->isChecked());
	///TODO
}

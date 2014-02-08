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


#include "preferencedialog.h"

#include <wrappers/translation.h>

#include <models/modelkeyboard.h>
#include <models/modelshortcutmap.h>
#include <models/modelmain.h>

#include <gui/core/keyboardhandler.h>
#include <gui/widgets/captionwidget.h>
#include <gui/widgets/modifierkeyswidget.h>
#include <gui/widgets/keyboardwidget.h>
#include <gui/widgets/timedurationwidget.h>

#include <functional>

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTabWidget>
#include <QVBoxLayout>


// Constructor.
PreferenceDialog::PreferenceDialog(QWidget *parent) : QDialog(parent),
	_captionToggledShunt(false), _selectedCaption(nullptr), _selectedShortcut(0)
{
	// Top-level layout
	setWindowTitle(_("Preferences"));
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	// Tabs
	_tabWidget = new QTabWidget(this);
	mainLayout->addWidget(_tabWidget, 1);
	_tabWidget->addTab(createKeyboardPage     (), _("Keyboard set-up"));
	_tabWidget->addTab(createTimeDisplayPage  (), _("Time display"   ));
	_tabWidget->addTab(createMiscellaneousPage(), _("Miscellaneous"  ));
	connect(_tabWidget, &QTabWidget::currentChanged, this, &PreferenceDialog::refreshKeyboardHandlerActivationState);
	refreshKeyboardHandlerActivationState();

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
	_keyboardWidget->bindShortcutMap(_shortcutMap);
	connect(_keyboardWidget, &KeyboardWidget::keyClicked, this, &PreferenceDialog::onKeyClicked);
	layout->addWidget(_keyboardWidget, 1);

	// Captions (exception modifier keys)
	CaptionWidget *captionLeft  = captionWidgetFactory(QColor(  0,176,  0), _("Left player's button" ), 1);
	CaptionWidget *captionRight = captionWidgetFactory(QColor(  0,128,255), _("Right player's button"), 2);
	CaptionWidget *captionPause = captionWidgetFactory(QColor(255,128,  0), _("Pause"                ), 3);
	CaptionWidget *captionReset = captionWidgetFactory(QColor(240, 48,255), _("Reset"                ), 4);
	CaptionWidget *captionSwap  = captionWidgetFactory(QColor(255, 16, 16), _("Switch"               ), 5);
	QGridLayout *captionLayout = new QGridLayout;
	captionLayout->addWidget(captionLeft , 0, 0);
	captionLayout->addWidget(captionRight, 1, 0);
	captionLayout->addWidget(captionPause, 0, 1);
	captionLayout->addWidget(captionReset, 1, 1);
	captionLayout->addWidget(captionSwap , 0, 2);

	// Modifier keys selector
	_modifierKeysSelector = new ModifierKeysWidget(this);
	_keyboardWidget->setModifierKeyBrush(_modifierKeysSelector->color());
	connect(_modifierKeysSelector, &ModifierKeysWidget::valueChanged, this, &PreferenceDialog::onModifierKeysChanged);
	captionLayout->addWidget(_modifierKeysSelector, 1, 2);

	// Shortcut mode selector
	_shortcutModeSelector = new QPushButton(this);
	_shortcutModeSelector->setCheckable(true);
	_shortcutModeSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(_shortcutModeSelector, &QPushButton::toggled, this, &PreferenceDialog::onShortcutModeChanged);

	// Layout for the captions and the modifier keys toggle
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addLayout(captionLayout        , 5);
	bottomLayout->addWidget(_shortcutModeSelector, 2);
	layout->addLayout(bottomLayout);

	// Ensure that the widget's states are coherent.
	onHasNumericKeypadToggled();
	onModifierKeysChanged    ();
	onShortcutModeChanged    ();

	// Tool-tips
	_keyboardWidget->setToolTip(QString("<p><b>%1</b></p><p>%2</p><p>%3</p>")
		.arg(_("Key actions"))
		.arg(_(
			"This schematic representation of the keyboard shows the actions that are associated to each key."
		))
		.arg(_(
			"To associate an action to a key, click on the corresponding caption "
			"below the keyboard representation, and then click on the key "
			"whose behavior you want to change. "
			"Clicking twice on a key cancels the key/action association."
		))
	);
	_shortcutModeSelector->setToolTip(QString("<p>%1</p><p>%2</p>")
		.arg(_(
			"Two actions may be associated to each key: "
			"one is triggered by default when the user presses the key, "
			"the other is triggered when the user presses the key while holding "
			"both &quot;modifier keys&quot; down. "
			"For instance, in the default configuration, the modifier keys are the shift keys, "
			"and pressing &lt;shift-left&gt; + &lt;shift-right&gt; + &lt;space&gt; "
			"stops the clock."
		))
		.arg(_(
			"Toggle this button to show (and edit) either the default actions "
			"or the actions triggered when the modifier keys are pressed."
		))
	);
	captionLeft->button()->setToolTip("<p>" + _(
		"Edit the keys that the left player can press to start the right player's clock."
	) + "</p>");
	captionRight->button()->setToolTip("<p>" + _(
		"Edit the keys that the right player can press to start the left player's clock."
	) + "</p>");
	captionPause->button()->setToolTip("<p>" + _("Edit the keys that can be pressed to stop the clock." ) + "</p>");
	captionReset->button()->setToolTip("<p>" + _("Edit the keys that can be pressed to reset the clock.") + "</p>");
	captionSwap ->button()->setToolTip("<p>" + _(
		"Edit the keys that can be pressed to swap the players and the time control parameters."
	) + "</p>");
	_modifierKeysSelector->setToolTip(_("Click to change the modifier keys."));

	// Return the page widget
	return page;
}


// Caption widget factory method.
CaptionWidget *PreferenceDialog::captionWidgetFactory(const QColor &color, const QString &label, int shortcut)
{
	CaptionWidget *widget = new CaptionWidget(color, label, this);
	widget->button()->setCheckable(true);
	connect(widget->button(), &QToolButton::toggled, std::bind(&PreferenceDialog::onCaptionToggled, this, widget, shortcut));
	_keyboardWidget->shortcutColors()[shortcut] = color;
	return widget;
}


// Create the time-display page.
QWidget *PreferenceDialog::createTimeDisplayPage()
{
	// Page widget
	QWidget *page = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout;
	page->setLayout(layout);

	// Delay before display seconds
	_delayBeforeDisplaySeconds = new TimeDurationWidget(this);
	QLabel      *dbdsLabel  = new QLabel(_("Show seconds only if the time is less than"), this);
	QHBoxLayout *dbdsLayout = new QHBoxLayout;
	dbdsLayout->addWidget(dbdsLabel);
	dbdsLayout->addWidget(_delayBeforeDisplaySeconds);
	dbdsLayout->addStretch(1);
	layout->addLayout(dbdsLayout);

	// Check-boxes
	_displayTimeAfterTimeout   = new QCheckBox(_("Display an increasing time counter when the flag is down"     ), this);
	_displayBronsteinExtraInfo = new QCheckBox(_("Display extra time information when playing in Bronstein mode"), this);
	_displayByoYomiExtraInfo   = new QCheckBox(_("Display extra time information when playing in byo-yomi mode" ), this);
	layout->addWidget(_displayTimeAfterTimeout  );
	layout->addWidget(_displayBronsteinExtraInfo);
	layout->addWidget(_displayByoYomiExtraInfo  );

	// Tool-tips
	_delayBeforeDisplaySeconds->setToolTip("<p>" + _(
		"If the remaining time is greater than (or equal to) this threshold, "
		"it is rounded to the nearest minute before being displayed. For instance, "
		"if the threshold is set to 0, the seconds are never displayed."
	) + "</p><p>" + _(
		"This parameter affects only the display, not the accuracy of the time measurement."
	) + "</p>");
	_displayTimeAfterTimeout->setToolTip("<p>" + _(
		"If checked, an increasing time counter is displayed when the players run out of time. "
		"Otherwise, in this situation, the clock only displays \"Flag down\"."
	) + "</p>");
	_displayBronsteinExtraInfo->setToolTip("<p>" + _(
		"This option only affects the display with the Bronstein time control mode. "
		"If checked, the main time and the additional time are displayed separately."
	) + "</p>");
	_displayByoYomiExtraInfo->setToolTip("<p>" + _(
		"This option only affects the display with the byo-yomi time control mode. "
		"If checked, the clock displays when the players are currently spending "
		"their additional byo-yomi periods."
	) + "</p>");
	dbdsLabel->setToolTip(_delayBeforeDisplaySeconds->toolTip());

	// Return the page widget
	layout->addStretch(1);
	return page;
}


// Create the miscellaneous page.
QWidget *PreferenceDialog::createMiscellaneousPage()
{
	// Page widget
	QWidget *page = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout;
	page->setLayout(layout);

	// Status bar
	_showStatusBar = new QCheckBox(_("Show the status bar at the bottom of the main window"), this);
	layout->addWidget(_showStatusBar);

	// Reset confirmation labels
	Enum::array<ResetConfirmation, QString> rcLabel;
	rcLabel[ResetConfirmation::ALWAYS   ] = _("Always");
	rcLabel[ResetConfirmation::IF_ACTIVE] = _("Only if the clock is still running");
	rcLabel[ResetConfirmation::NEVER    ] = _("Never");

	// Reset confirmation option
	QGroupBox   *rcGroup  = new QGroupBox(_("Ask for confirmation when clicking on the \"Reset the clock\" button?"), this);
	QVBoxLayout *rcLayout = new QVBoxLayout;
	layout->addWidget(rcGroup);
	rcGroup->setLayout(rcLayout);
	for(auto it=Enum::cursor<ResetConfirmation>::first(); it.valid(); ++it) {
		_resetConfirmation[*it] = new QRadioButton(rcLabel[*it], this);
		rcLayout->addWidget(_resetConfirmation[*it]);
	}

	// Return the page widget
	layout->addStretch(1);
	return page;
}


// Window state-change handler.
void PreferenceDialog::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::ActivationChange) {
		refreshKeyboardHandlerActivationState();
	}
	QDialog::changeEvent(event);
}


// Activate or in-activate the keyboard handler depending on the state of the dialog and the active tab.
void PreferenceDialog::refreshKeyboardHandlerActivationState()
{
	bool shouldBeEnabled = isActiveWindow() && _tabWidget->currentIndex()==0;
	_keyboardHandler->setEnabled(shouldBeEnabled);
}


// Feed the keyboard selector combo-box.
void PreferenceDialog::feedKeyboardSelector()
{
	for(const auto &it : ModelKeyboard::instance().ids()) {
		_keyboardSelector->addItem(
			ModelKeyboard::instance().icon(it),
			QString::fromStdString(ModelKeyboard::instance().name(it)),
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
	_keyboardWidget->bindKeyboardMap(ModelKeyboard::instance().keyboard_map(retrieveSelectedKeyboard()));
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


// Action performed when the state of the shortcut mode selector is changed.
void PreferenceDialog::onShortcutModeChanged()
{
	bool showShortcutHigh = _shortcutModeSelector->isChecked();
	_shortcutModeSelector->setText(showShortcutHigh ?
		_("Key actions\nwhen the modifier keys\nare pressed") :
		_("Default\nkey actions")
	);
	_keyboardWidget->setShowShortcutHigh(showShortcutHigh);
}


// Action performed when a caption button is toggled.
void PreferenceDialog::onCaptionToggled(CaptionWidget *widget, int shortcut)
{
	if(_captionToggledShunt) {
		return;
	}

	// Un-toggle the current selected caption, if any.
	if(_selectedCaption!=nullptr && _selectedCaption!=widget) {
		_captionToggledShunt = true;
		_selectedCaption->button()->setChecked(false);
		_captionToggledShunt = false;
	}

	// Update the selected caption pointer.
	_selectedCaption  = widget->button()->isChecked() ? widget   : nullptr;
	_selectedShortcut = widget->button()->isChecked() ? shortcut : 0;
}


// Action performed when the user clicks on the graphic representation of a key in the keyboard widget.
void PreferenceDialog::onKeyClicked(const std::string &id, Qt::MouseButton button)
{
	// Nothing to do if no caption is selected or if it was not the left button.
	if(_selectedShortcut==0 || button!=Qt::LeftButton) {
		return;
	}

	// Get the index of the shortcut currently associated to the key with the given ID.
	bool useShortcutHigh = _shortcutModeSelector->isChecked();
	int  currentShortcut = _shortcutMap.shortcut(id, useShortcutHigh);

	// Update the shortcut associated to the key.
	_shortcutMap.set_shortcut(id, useShortcutHigh, currentShortcut==_selectedShortcut ? 0 : _selectedShortcut);
}


// Load the dialog with the parameters saved in the Params singleton object.
void PreferenceDialog::loadParameters()
{
	ModelMain &model(ModelMain::instance());

	// Keyboard page
	_keyboardSelector->setCurrentIndex(_keyboardSelector->findData(QVariant(model.keyboard_id().c_str())));
	_modifierKeysSelector->setModifierKeys(model.modifier_keys              ());
	_hasNumericKeypad    ->setChecked     (model.keyboard_has_numeric_keypad());
	_shortcutMap = ModelShortcutMap::instance().shortcut_map();

	// Time display page
	_delayBeforeDisplaySeconds->setValue  (model.delay_before_display_seconds());
	_displayTimeAfterTimeout  ->setChecked(model.display_time_after_timeout  ());
	_displayBronsteinExtraInfo->setChecked(model.display_bronstein_extra_info());
	_displayByoYomiExtraInfo  ->setChecked(model.display_byo_yomi_extra_info ());

	// Miscellaneous page
	_showStatusBar->setChecked(model.show_status_bar());
	_resetConfirmation[model.reset_confirmation()]->setChecked(true);
}


// Save the new parameters defined from the dialog in the Params singleton object.
void PreferenceDialog::saveParameters()
{
	ModelMain &model(ModelMain::instance());

	// Keyboard page
	model.keyboard_id(retrieveSelectedKeyboard());
	model.modifier_keys              (_modifierKeysSelector->modifierKeys());
	model.keyboard_has_numeric_keypad(_hasNumericKeypad    ->isChecked   ());
	ModelShortcutMap::instance().shortcut_map(_shortcutMap);

	// Time display page
	model.delay_before_display_seconds(_delayBeforeDisplaySeconds->value    ());
	model.display_time_after_timeout  (_displayTimeAfterTimeout  ->isChecked());
	model.display_bronstein_extra_info(_displayBronsteinExtraInfo->isChecked());
	model.display_byo_yomi_extra_info (_displayByoYomiExtraInfo  ->isChecked());

	// Miscellaneous page
	model.show_status_bar(_showStatusBar->isChecked());
	for(auto it=Enum::cursor<ResetConfirmation>::first(); it.valid(); ++it) {
		if(_resetConfirmation[*it]->isChecked()) {
			model.reset_confirmation(*it);
		}
	}
}

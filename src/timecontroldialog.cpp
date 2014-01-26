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


#include "timecontroldialog.h"
#include "timedurationwidget.h"
#include <wrappers/translation.h>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


// Constructor.
TimeControlDialog::TimeControlDialog(QWidget *parent) : QDialog(parent), _shuntSignal(true)
{
	// Top-level layout
	setWindowTitle(_("Time control"));
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	// Tool-tips on time control modes
	Enum::array<TimeControl::Mode, std::string> modeDescription;
	modeDescription[TimeControl::Mode::SUDDEN_DEATH] = _(
		"Simplest time control mode. No additional delay is never added to the "
		"player's main time."
	);
	modeDescription[TimeControl::Mode::FISCHER] = _(
		"At each move, the player is granted an additional delay, so that his/her "
		"global clock time is always greater than this bonus time at the "
		"beginning of his/her playing turn. Additional delays can be "
		"accumulated."
	);
	modeDescription[TimeControl::Mode::BRONSTEIN] = _(
		"Same as Fischer time control mode, except that additional delays cannot "
		"be accumulated. Bronstein mode can also be thought as a kind of sudden "
		"death mode in which the clock waits for a certain delay before starting "
		"to substract from the player's remaining time when engaged."
	);
	modeDescription[TimeControl::Mode::HOURGLASS] = _(
		"While the current player is deciding on his/her move, the opponent's "
		"time is increasing. Therefore, the sum of both clocks remains always the "
		"same."
	);
	modeDescription[TimeControl::Mode::BYO_YOMI] = _(
		"When the player's main time expires, he/she is granted one or more "
		"additional time periods, denoted as \"byo-yomi\" periods. If the move is "
		"completed before the current byo-yomi expires, the clock is reset with "
		"the whole byo-yomi time available for the next move; otherwise, the "
		"next byo-yomi starts. At the end of the last byo-yomi, the game is lost."
	);

	// Selection of the time control mode
	QGroupBox *modeGroup = new QGroupBox(_("Mode"), this);
	mainLayout->addWidget(modeGroup);
	QVBoxLayout *modeLayout = new QVBoxLayout;
	modeGroup->setLayout(modeLayout);
	for(auto it=Enum::cursor<TimeControl::Mode>::first(); it.valid(); ++it)
	{
		// Create the radio buttons
		_mode[*it] = new QRadioButton(QString::fromStdString(TimeControl::mode_name(*it)), this);
		connect(_mode[*it], &QCheckBox::toggled, boost::bind(&TimeControlDialog::onModeButtonToggled, this, _mode[*it]));
		modeLayout->addWidget(_mode[*it]);

		// Tool-tips
		static boost::format modeToolTipFormater("<p><b>%1%</b></p><p>%2%</p>");
		std::string toolTip = boost::str(modeToolTipFormater.clear() % TimeControl::mode_name(*it) % modeDescription[*it]);
		_mode[*it]->setToolTip(QString::fromStdString(toolTip));
	}

	// Check-box "same times on both sides"
	_identicTimes = new QCheckBox(_("Same time for both sides"), this);
	connect(_identicTimes, &QCheckBox::toggled, this, &TimeControlDialog::onIdenticTimesToggled);
	mainLayout->addWidget(_identicTimes);

	// Time options
	QHBoxLayout *timeLayout = new QHBoxLayout;
	mainLayout->addLayout(timeLayout);
	for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it)
	{
		// Widget for the time options relative to one player
		QGroupBox *player = new QGroupBox(*it==Side::LEFT ? _("Left") : _("Right"), this);
		QGridLayout *playerLayout = new QGridLayout;
		player->setLayout(playerLayout);
		timeLayout->addWidget(player, 1);

		// Labels
		playerLayout->addWidget(new QLabel(_("Main time"       ), this), 0, 0, Qt::AlignCenter);
		playerLayout->addWidget(new QLabel(_("Increment"       ), this), 1, 0, Qt::AlignCenter);
		playerLayout->addWidget(new QLabel(_("Byo-yomi periods"), this), 2, 0, Qt::AlignCenter);

		// Fields
		_mainTime  [*it] = new TimeDurationWidget(this);
		_increment [*it] = new TimeDurationWidget(this);
		_byoPeriods[*it] = new QSpinBox(this);
		playerLayout->addWidget(_mainTime  [*it], 0, 1);
		playerLayout->addWidget(_increment [*it], 1, 1);
		playerLayout->addWidget(_byoPeriods[*it], 2, 1);
		playerLayout->setColumnStretch(1, 1);
	}
	connect(_mainTime  [Side::LEFT], &TimeDurationWidget::valueChanged                           , this, &TimeControlDialog::copyLeftMainTime  );
	connect(_increment [Side::LEFT], &TimeDurationWidget::valueChanged                           , this, &TimeControlDialog::copyLeftIncrement );
	connect(_byoPeriods[Side::LEFT], static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &TimeControlDialog::copyLeftByoPeriods);

	// Validation buttons
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	mainLayout->addStretch(1);
	mainLayout->addWidget(buttons);

	// Initial state
	_mode[TimeControl::Mode::SUDDEN_DEATH]->setChecked(true);
	_identicTimes->setChecked(true);
	refreshEnableStates();
	_shuntSignal = false;
}


// Return the time control defined in the dialog.
TimeControl TimeControlDialog::timeControl() const
{
	// Retrieve the selected time control mode.
	TimeControl retval;
	for(auto it=Enum::cursor<TimeControl::Mode>::first(); it.valid(); ++it) {
		if(_mode[*it]->isChecked()) {
			retval.set_mode(*it);
		}
	}

	// Retrieve the value of the time fields.
	for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it) {
		retval.set_main_time  (*it, _mainTime  [*it]->value());
		retval.set_increment  (*it, _increment [*it]->value());
		retval.set_byo_periods(*it, _byoPeriods[*it]->value());
	}

	// Return the result.
	return std::move(retval);
}


// Initialize the time control in the dialog.
void TimeControlDialog::setTimeControl(const TimeControl &value)
{
	// Set the time control mode radio and the 'identic-times' checkbox.
	_shuntSignal = true;
	_mode[value.mode()]->setChecked(true);
	_identicTimes->setChecked(value.both_sides_have_same_time());
	_shuntSignal = false;

	// Set the side-related fields
	for(auto it=Enum::cursor<Side>::first(); it.valid(); ++it) {
		_mainTime  [*it]->setValue(value.main_time  (*it));
		_increment [*it]->setValue(value.increment  (*it));
		_byoPeriods[*it]->setValue(value.byo_periods(*it));
	}

	// Update the activation state of the widgets
	refreshEnableStates();
}


// Handler for the time control mode radio buttons.
void TimeControlDialog::onModeButtonToggled(QRadioButton *button)
{
	if(_shuntSignal || !button->isChecked()) {
		return;
	}
	refreshEnableStates();
}


// Handler for the 'identic-times' checkbox.
void TimeControlDialog::onIdenticTimesToggled()
{
	if(_shuntSignal) {
		return;
	}
	refreshEnableStates();
	copyLeftMainTime   ();
	copyLeftIncrement  ();
	copyLeftByoPeriods ();
}


// Update the activation state of the widgets according to their values.
void TimeControlDialog::refreshEnableStates()
{
	bool enableIncrement = (
		_mode[TimeControl::Mode::FISCHER  ]->isChecked() ||
		_mode[TimeControl::Mode::BRONSTEIN]->isChecked() ||
		_mode[TimeControl::Mode::BYO_YOMI ]->isChecked()
	);
	bool enableByoPeriods = _mode[TimeControl::Mode::BYO_YOMI]->isChecked();
	bool enableRight      = !_identicTimes->isChecked();
	_increment [Side::LEFT ]->setEnabled(enableIncrement );
	_byoPeriods[Side::LEFT ]->setEnabled(enableByoPeriods);
	_mainTime  [Side::RIGHT]->setEnabled(                    enableRight);
	_increment [Side::RIGHT]->setEnabled(enableIncrement  && enableRight);
	_byoPeriods[Side::RIGHT]->setEnabled(enableByoPeriods && enableRight);
}


// Copy the value in the left fields into the corresponding right fields if the 'identic-times' option is toggled.
void TimeControlDialog::copyLeftMainTime  () { if(_identicTimes->isChecked()) _mainTime  [Side::RIGHT]->setValue(_mainTime  [Side::LEFT]->value()); }
void TimeControlDialog::copyLeftIncrement () { if(_identicTimes->isChecked()) _increment [Side::RIGHT]->setValue(_increment [Side::LEFT]->value()); }
void TimeControlDialog::copyLeftByoPeriods() { if(_identicTimes->isChecked()) _byoPeriods[Side::RIGHT]->setValue(_byoPeriods[Side::LEFT]->value()); }

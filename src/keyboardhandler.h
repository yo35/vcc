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


#ifndef KEYBOARDHANDLER_H_
#define KEYBOARDHANDLER_H_

#include <QObject>
#include <cstdint>
#include <set>

#include <QAbstractNativeEventFilter>
class xcb_connection_t;
class xcb_screen_t    ;


/**
 * Object to capture and to handle all the key-press/key-release events in a low-level manner.
 *
 * This low-level handling is particularly useful to deal with dead-keys
 * (such as the circumflex key ^ in the French keyboard) or with keys that triggers an action
 * at the OS level (for instance, pressing the window key on Windows popus the start menu
 * and makes the application lost the focus).
 */
class KeyboardHandler : public QObject
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	KeyboardHandler(QObject *parent);

	/**
	 * Destructor.
	 */
	virtual ~KeyboardHandler();

	/**
	 * Whether the keyboard handler is actually enabled or not. If not enabled,
	 * no signal will be emitted by the keyboard handler, and no key is reported
	 * as "down". By default, the keyboard handler is not enabled.
	 */
	bool isEnabled() const { return _enabled; }

	/**
	 * Enable or disable the keyboard handler.
	 */
	void setEnabled(bool enabled);

	/**
	 * Return a set containing the scan-codes of all keys currently down.
	 */
	const std::set<std::uint32_t> &keysDown() const { return _keysDown; }

	/**
	 * Check whether a given key is or not currently down.
	 */
	bool isDown(std::uint32_t scanCode) const { return _keysDown.count(scanCode)>0; }

signals:

	/**
	 * Signal emitted when a key is pressed.
	 */
	void keyPressed(std::uint32_t scanCode);

	/**
	 * Signal emitted when a key is released.
	 */
	void keyReleased(std::uint32_t scanCode);

private:

	/**
	 * Event filter used to alter the behavior ot the regular Qt event dispatcher.
	 */
	class EventFilter : public QAbstractNativeEventFilter
	{
	public:

		/**
		 * Constructor.
		 */
		EventFilter(KeyboardHandler *owner);

		/**
		 * Implementation of the event filter method.
		 */
		bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

	private:

		// Private members
		KeyboardHandler *_owner;
	};

	// Private functions
	void notifyKeyPressed (std::uint32_t scanCode);
	void notifyKeyReleased(std::uint32_t scanCode);
	void clearKeysDown();

	// Private members
	bool                    _enabled ;
	std::set<std::uint32_t> _keysDown;

	// XCB implementation
	EventFilter       _eventFilter;
	xcb_connection_t *_connection ;
	xcb_screen_t     *_screen     ;
};

#endif /* KEYBOARDHANDLER_H_ */

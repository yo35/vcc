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
#include <QWidget>
#include <set>
#include <core/keys.h>

#ifdef Q_OS_WIN
	#include <windows.h>
#else
	#include <QAbstractNativeEventFilter>
#endif


/**
 * Object to capture and to handle all the key-press/key-release events in a low-level manner.
 *
 * This low-level handling is particularly useful to deal with dead-keys
 * (such as the circumflex key ^ in the French keyboard) or with keys that triggers an action
 * at the OS level (for instance, pressing the window key on Windows popups the start menu
 * and makes the application lost the focus).
 */
class KeyboardHandler : public QObject
{
	Q_OBJECT

public:

	/**
	 * Constructor.
	 */
	KeyboardHandler(QWidget *parent);

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
	const std::set<ScanCode> &keysDown() const { return _keysDown; }

	/**
	 * Check whether a given key is or not currently down.
	 */
	bool isDown(ScanCode scanCode) const { return _keysDown.count(scanCode)>0; }

signals:

	/**
	 * Signal emitted when a key is pressed.
	 */
	void keyPressed(ScanCode scanCode);

	/**
	 * Signal emitted when a key is released.
	 */
	void keyReleased(ScanCode scanCode);

private:

	#ifdef Q_OS_WIN

		/**
		 * Pointer to the lastly activated keyboard handler.
		 */
		static KeyboardHandler *_activeHandler;

		/**
		 * Low-level callback method to handle keyboard events.
		 */
		static LRESULT CALLBACK lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

	#else

		/**
		 * Event filter used to alter the behavior of the regular Qt event dispatcher.
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

	#endif

	// Private functions
	void notifyKeyPressed (ScanCode scanCode);
	void notifyKeyReleased(ScanCode scanCode);
	void clearKeysDown();

	// Private members
	bool               _enabled ;
	std::set<ScanCode> _keysDown;

	// OS-dependent implementation
	#ifdef Q_OS_WIN
		HMODULE _hModule;
		HHOOK   _hHook  ;
	#else
		EventFilter _eventFilter;
		QWidget    *_parent     ;
	#endif
};

#endif /* KEYBOARDHANDLER_H_ */

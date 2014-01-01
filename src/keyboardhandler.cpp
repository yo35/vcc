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


#include "keyboardhandler.h"
#include <QWidget>
#ifndef Q_OS_WIN
	#include <QAbstractEventDispatcher>
	#include <QX11Info>
	#include <xcb/xcb.h>
#endif


// Emit a key-press event corresponding the given scan code if the corresponding
// key is not already down (to avoid auto-repeat events).
void KeyboardHandler::notifyKeyPressed(std::uint32_t scanCode)
{
	if(_keysDown.count(scanCode)>0) {
		return;
	}
	_keysDown.insert(scanCode);
	emit keyPressed(scanCode);
}


// Emit a key-release event corresponding the given scan code
// if the corresponding key is actually down.
void KeyboardHandler::notifyKeyReleased(std::uint32_t scanCode)
{
	if(_keysDown.count(scanCode)==0) {
		return;
	}
	_keysDown.erase(scanCode);
	emit keyReleased(scanCode);
}


// Make the "keys-down" set empty and generate the corresponding key-release events.
void KeyboardHandler::clearKeysDown()
{
	std::set<std::uint32_t> currentlyDown(_keysDown);
	for(std::uint32_t key : currentlyDown) {
		notifyKeyReleased(key);
	}
}


// Constructor.
KeyboardHandler::KeyboardHandler(QWidget *parent) : QObject(parent), _enabled(false),
	#ifdef Q_OS_WIN
		_hModule(GetModuleHandle(NULL)), _hHook(NULL)
	#else
		_eventFilter(this), _parent(parent)
	#endif
{
	#ifndef Q_OS_WIN
		QAbstractEventDispatcher::instance()->installNativeEventFilter(&_eventFilter);
	#endif
}


// Destructor.
KeyboardHandler::~KeyboardHandler()
{
	setEnabled(false);
	#ifndef Q_OS_WIN
		QAbstractEventDispatcher::instance()->removeNativeEventFilter(&_eventFilter);
	#endif
}


// Enable or disable the keyboard handler.
void KeyboardHandler::setEnabled(bool enabled)
{
	// Nothing to do if already in the requested state.
	if(_enabled==enabled) {
		return;
	}
	_enabled = enabled;

	// Enable the keyboard handler.
	if(_enabled)
	{
		// Grab the keyboard to avoid unexpected key sequences being intercepted by the OS
		// (for instance, it avoids the main menu begin rolled down in the Cinnamon desktop).
		#ifdef Q_OS_WIN
			_activeHandler = this;
			_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHandler::lowLevelKeyboardProc, _hModule, 0);
		#else
			_parent->grabKeyboard();
		#endif
	}

	// Disable the keyboard handler.
	else {
		#ifdef Q_OS_WIN
			UnhookWindowsHookEx(_hHook);
			_hHook = NULL;
		#else
			_parent->releaseKeyboard();
		#endif
		clearKeysDown();
	}
}


#ifdef Q_OS_WIN

// Pointer to the lastly activated keyboard handler.
KeyboardHandler *KeyboardHandler::_activeHandler = nullptr;


// Low-level callback method to handle keyboard events.
LRESULT CALLBACK KeyboardHandler::lowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// nCode<0 has a special meaning, as specified by the MSDN.
	// However, this case seems to never (or rarely) happens.
	if(nCode<0) {
		return CallNextHookEx(0, nCode, wParam, lParam);
	}

	// Extract the scan-code.
	KBDLLHOOKSTRUCT *info = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
	std::uint32_t scanCode = info->scanCode;
	if(info->flags & 0x01) {
		scanCode += 256;
	}

	// Notify the keyboard handler.
	switch(wParam)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			_activeHandler->notifyKeyPressed(scanCode);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			_activeHandler->notifyKeyReleased(scanCode);
			break;
	}
	return 1;
}


#else


// Event filter constructor.
KeyboardHandler::EventFilter::EventFilter(KeyboardHandler *owner) : _owner(owner) {}


// Implementation of the event filter method.
bool KeyboardHandler::EventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
	// Do not intercept anything if the keyboard handler is disabled.
	if(!_owner->_enabled) {
		return false;
	}

	// Intercept only XCB events (although no other type of event is expected).
	if(eventType!="xcb_generic_event_t") {
		return false;
	}
	xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);

	// Redirect key-press and key-release events to the notification methods of the owner,
	// and forward everything else to the regular event dispatcher.
	switch(event->response_type)
	{
		case XCB_KEY_PRESS:
			_owner->notifyKeyPressed(reinterpret_cast<xcb_key_press_event_t *>(event)->detail);
			return true;

		case XCB_KEY_RELEASE:
			_owner->notifyKeyReleased(reinterpret_cast<xcb_key_release_event_t *>(event)->detail);
			return true;

		default:
			return false;
	}
}

#endif /* Q_OS_WIN */

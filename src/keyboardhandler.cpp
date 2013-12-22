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
#include <QAbstractEventDispatcher>
#include <QX11Info>
#include <xcb/xcb.h>

#include <iostream>


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


// Constructor.
KeyboardHandler::KeyboardHandler(QWidget *parent) : QObject(parent), _eventFilter(this)
{
	// Install the event filter that will intercept key events.
	QAbstractEventDispatcher::instance()->installNativeEventFilter(&_eventFilter);

	// Grab the keyboard to avoid unexpected key sequences being intercepted by the OS
	// (for instance, it avoids the main menu begin rolled down in the Cinnamon desktop).
	_connection = QX11Info::connection();
	_screen     = xcb_setup_roots_iterator(xcb_get_setup(_connection)).data;
	xcb_grab_keyboard(_connection, true, _screen->root, XCB_CURRENT_TIME, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
}


// Destructor.
KeyboardHandler::~KeyboardHandler()
{
	xcb_ungrab_keyboard(_connection, XCB_CURRENT_TIME);
	QAbstractEventDispatcher::instance()->removeNativeEventFilter(&_eventFilter);
}


// Event filter constructor.
KeyboardHandler::EventFilter::EventFilter(KeyboardHandler *owner) : _owner(owner) {}


// Implementation of the event filter method.
bool KeyboardHandler::EventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
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
			std::cout << "Key pressed: " << static_cast<int>(reinterpret_cast<xcb_key_press_event_t *>(event)->detail) << std::endl;
			return true;

		case XCB_KEY_RELEASE:
			_owner->notifyKeyReleased(reinterpret_cast<xcb_key_release_event_t *>(event)->detail);
			std::cout << "Key released: " << static_cast<int>(reinterpret_cast<xcb_key_press_event_t *>(event)->detail) << std::endl;
			return true;

		default:
			return false;
	}
}

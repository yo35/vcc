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


#include "winkeyhookdll.h"
#include <windows.h>
#include <config.h>

HHOOK hHook = 0;

LRESULT CALLBACK low_level_keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT kb_input_event;

	/* First check */
	if(nCode<0)
		return CallNextHookEx(0, nCode, wParam, lParam);
	kb_input_event = (PKBDLLHOOKSTRUCT)lParam;

	/* If the event comes from a window key, don't do anything */
	if(kb_input_event->vkCode==VK_LWIN || kb_input_event->vkCode==VK_RWIN)
		return 1;

	/* Otherwise, forward the event */
	else
		return CallNextHookEx(0, nCode, wParam, lParam);
}

int __declspec(dllexport) set_kbd_hook()
{
	HMODULE hCurrModule;

	/* Retrieve current module's handle */
	hCurrModule = GetModuleHandle(HOOKDLL_FULL_NAME);
	if(hCurrModule==NULL)
		return GetLastError();

	/* Set the hook function */
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, low_level_keyboard_proc, hCurrModule, 0);
	if(hHook==NULL)
		return GetLastError();

	return -1;
}

void __declspec(dllexport) unset_kbd_hook()
{
	/* Unset the hook function */
	UnhookWindowsHookEx(hHook);
}

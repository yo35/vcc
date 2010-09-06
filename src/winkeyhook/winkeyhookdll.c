
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

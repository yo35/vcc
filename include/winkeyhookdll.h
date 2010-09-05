
#ifndef WINKEYHOOKDLL_H
#define WINKEYHOOKDLL_H

#ifdef __cplusplus
extern "C" {
#endif

int  __declspec(dllexport)   set_kbd_hook();
void __declspec(dllexport) unset_kbd_hook();

#ifdef __cplusplus
}
#endif

#endif

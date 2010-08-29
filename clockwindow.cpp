
#include "clockwindow.h"
#include <iostream>

ClockWindow::ClockWindow() : Gtk::Window() {
	for(int i=0; i<2; ++i) {
		timer[i].set_time(3*60*1000);
		//timer[i].set_time(3*1000);
		timer[i].set_mode(Timer::PAUSED);
		dial [i].set_timer(timer[i]);
		layout.pack_start(dial[i]);
	}
	no_actif = -1;
	set_events(Gdk::KEY_PRESS_MASK | Gdk::BUTTON_PRESS_MASK);
	add(layout);
	show_all_children();
}

#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70
#define KEY_G 71
#define KEY_H 72
#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_M 77
#define KEY_N 78
#define KEY_O 79
#define KEY_P 80
#define KEY_Q 81
#define KEY_R 82
#define KEY_S 83
#define KEY_T 84
#define KEY_U 85
#define KEY_V 86
#define KEY_W 87
#define KEY_X 88
#define KEY_Y 89
#define KEY_Z 90
#define KEY_TAB         9
#define KEY_CAPS_LOCK   20
#define KEY_MAJ_L       16
#define KEY_CTRL_L      17
#define KEY_ALT_L       18
#define KEY_WIN_L       91
#define KEY_MENU        93
#define KEY_ALT_R       165
#define KEY_CTRL_R      163
#define KEY_MAJ_R       161
#define KEY_ENTER       13
#define KEY_RETURN      8
#define KEY_ESCAPE      27
#define KEY_F1          112
#define KEY_F2          113
#define KEY_F3          114
#define KEY_F4          115
#define KEY_F5          116
#define KEY_F6          117
#define KEY_F7          118
#define KEY_F8          119
#define KEY_F9          120
#define KEY_F10         121
#define KEY_F11         122
#define KEY_F12         123
#define KEY_LEFT        37
#define KEY_TOP         38
#define KEY_RIGHT       39
#define KEY_BOTTOM      40
#define KEY_INSER       45
#define KEY_SUPPR       46
#define KEY_BEGIN       36
#define KEY_END         35
#define KEY_PAGE_PREV   33
#define KEY_PAGE_NEXT   34
#define KEY_0           48
#define KEY_1           49
#define KEY_2           50
#define KEY_3           51
#define KEY_4           52
#define KEY_5           53
#define KEY_6           54
#define KEY_7           55
#define KEY_8           56
#define KEY_9           57
#define KEY_EXPONENT    222
#define KEY_DEGREE      219
#define KEY_PLUS        187
#define KEY_CIRC        221
#define KEY_DOLLAR      186
#define KEY_POURCENT    192
#define KEY_STAR        220
#define KEY_ARROW       226
#define KEY_COMMA       188
#define KEY_SEMI_COLON  190
#define KEY_COLON       191
#define KEY_EXCLAM_MARK 223
#define KEY_PAUSE       19
#define KEY_SPACE       32

#define RIGHT_BUTTON 3
#define LEFT_BUTTON 1

bool ClockWindow::on_key_press_event(GdkEventKey* event)  {
	std::cout << "Button : " << event->hardware_keycode << std::endl;
	switch(event->hardware_keycode) {

		// Les blancs appuient sur la pendule
		case KEY_ESCAPE:
		case KEY_F1:
		case KEY_F2:
		case KEY_F3:
		case KEY_F4:
		case KEY_F5:
		case KEY_F6:
		case KEY_EXPONENT:
		case KEY_1:
		case KEY_2:
		case KEY_3:
		case KEY_4:
		case KEY_5:
		case KEY_TAB:
		case KEY_A:
		case KEY_Z:
		case KEY_E:
		case KEY_R:
		case KEY_T:
		case KEY_CAPS_LOCK:
		case KEY_Q:
		case KEY_S:
		case KEY_D:
		case KEY_F:
		case KEY_MAJ_L:
		case KEY_W:
		case KEY_X:
		case KEY_C:
		case KEY_V:
		case KEY_CTRL_L:
		case KEY_WIN_L:
		case KEY_ALT_L:
		case KEY_ARROW:
			set_no_actif(1);
			break;

		// Les noirs appuient sur la pendule
		case KEY_F10:
		case KEY_F11:
		case KEY_F12:
		case KEY_PAUSE:
		case KEY_INSER:
		case KEY_SUPPR:
		case KEY_9:
		case KEY_0:
		case KEY_DEGREE:
		case KEY_PLUS:
		case KEY_RETURN:
		case KEY_BEGIN:
		case KEY_I:
		case KEY_O:
		case KEY_P:
		case KEY_CIRC:
		case KEY_DOLLAR:
		case KEY_ENTER:
		case KEY_PAGE_PREV:
		case KEY_K:
		case KEY_L:
		case KEY_M:
		case KEY_POURCENT:
		case KEY_STAR:
		case KEY_PAGE_NEXT:
		case KEY_COMMA:
		case KEY_SEMI_COLON:
		case KEY_COLON:
		case KEY_EXCLAM_MARK:
		case KEY_MAJ_R:
		case KEY_TOP:
		case KEY_END:
		case KEY_ALT_R:
		case KEY_MENU:
		case KEY_CTRL_R:
		case KEY_LEFT:
		case KEY_BOTTOM:
		case KEY_RIGHT:
			set_no_actif(0);
			break;

		default:
			break;
	}
	return true;
}

void ClockWindow::set_no_actif(int new_no_actif) {
	if(new_no_actif==no_actif)
		return;

	if(no_actif>=0 && no_actif<2) {
		timer[no_actif].set_mode(Timer::PAUSED);
	}
	no_actif = new_no_actif;
	if(no_actif>=0 && no_actif<2) {
		timer[no_actif].set_mode(Timer::DECREMENT);
	}
}

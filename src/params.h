
#ifndef PARAMS_H
#define PARAMS_H

#include "timecontrol.h"
#include "keys.h"
#include "icon.h"
#include <set>

class Params {
public:

	// Constructeur
	Params();

	// Cadence de jeu
	TimeControl time_control;

	// Zones actives pour les touches clavier
	std::set<Keycode> key_area[2];
	void init_kb_areas(const KeyvalList &area_left, const KeyvalList &area_right);

	// Icones
	Icon icon_reset;
	Icon icon_pause;
	Icon icon_tctrl;

private:
	std::set<Keycode> aux_init_kb_areas(const KeyvalList &src);

};

extern Params *gp;

#endif


#ifndef PARAMS_H
#define PARAMS_H

#include "timecontrol.h"
#include "keymap.h"
#include <set>

class Params {
public:

	// Constructeur
	Params();

	// Cadence de jeu
	TimeControl time_control;

	// Zones actives pour les touches clavier
	std::set<Keymap::Keycode> key_area[2];
	void init_kb_areas(const Keymap::KeyvalList &area_left, const Keymap::KeyvalList &area_right);

private:
	std::set<Keymap::Keycode> aux_init_kb_areas(const Keymap::KeyvalList &src);

};



extern Params *gp;

#endif

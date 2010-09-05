
#include "params.h"
#include <cassert>
#include <fstream>
#include <string>

#define KEYMAP_PATH "keymap.txt"

Params *gp;

Params::Params() {

	// Cadence
	time_control.set_mode(TimeControl::BRONSTEIN);
	//time_control.set_mode(TimeControl::FISCHER);
	//time_control.set_mode(TimeControl::HOUR_GLASS);
	//time_control.set_mode(TimeControl::SIMPLE_DELAY);
	time_control.set_main_time(180*1000);
	time_control.set_increment(  3*1000);

	// Zones actives
	Keymap::KeyvalList  keyval_left ;
	Keymap::KeyvalList  keyval_right;
	Keymap::KeyvalList *curr_area = 0;
	std::ifstream       file;
	file.open(KEYMAP_PATH);
	while(!file.eof()) {
		std::string line;
		getline(file, line);

		// Cas d'une ligne vide
		if(line=="")
			continue;

		// Cas d'un changement de zone
		if(line=="LEFT")
			curr_area = &keyval_left;
		else if(line=="RIGHT")
			curr_area = &keyval_right;

		// Autre cas : on s'attend à un nombre
		else {
			int nb = 0;
			for(unsigned int k=0; k<line.length(); ++k) {
				assert(line[k]>='0' && line[k]<='9');
				nb = nb*10 + line[k] - '0';
			}
			assert(curr_area!=0);
			curr_area->push_back(nb);
		}
	}
	file.close();
	init_kb_areas(keyval_left, keyval_right);
}

void Params::init_kb_areas(const Keymap::KeyvalList &area_left, const Keymap::KeyvalList &area_right) {

	// Chargement des listes de touches gauches et droites
	key_area[0] = aux_init_kb_areas(area_left );
	key_area[1] = aux_init_kb_areas(area_right);

	// On vérifie que l'intersection des deux ensembles est vide
	std::set<Keymap::Keycode> area_both;
	area_both.insert(key_area[0].begin(), key_area[0].end());
	area_both.insert(key_area[1].begin(), key_area[1].end());
	assert(area_both.size()==key_area[0].size()+key_area[1].size());
}

std::set<Keymap::Keycode> Params::aux_init_kb_areas(const Keymap::KeyvalList &src) {
	std::set<Keymap::Keycode> retval;
	for(Keymap::KeyvalList::const_iterator it=src.begin(); it!=src.end(); ++it) {
		Keymap::KeycodeList keycodes = Keymap::keyval_to_keycodes(*it);
		retval.insert(keycodes.begin(), keycodes.end());
	}
	return retval;
}

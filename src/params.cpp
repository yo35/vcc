
#include "params.h"
#include <cassert>
#include <fstream>
#include <string>
#include <stdexcept>

#define KEYMAP_PATH "data/keymap.txt"

Params *gp;

Params::Params() :
	icon_reset("reset.svg"),
	icon_pause("pause.svg")
{

	// Cadence
	time_control.set_mode(TimeControl::FISCHER);
	time_control.set_main_time(180*1000);
	time_control.set_increment(  2*1000);

	// Zones actives
	KeyvalList    keyval_left ;
	KeyvalList    keyval_right;
	KeyvalList   *curr_area = 0;
	std::ifstream file;
	file.open(KEYMAP_PATH);
	if(file.fail())
		throw std::runtime_error("Unable to open the keyboard configuration file");
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
				if(line[k]<'0' || line[k]>'9')
					throw std::runtime_error("The keyboard configuration file is corrupted");
				nb = nb*10 + line[k] - '0';
			}
			if(curr_area==0)
				throw std::runtime_error("The keyboard configuration file is corrupted");
			curr_area->push_back(nb);
		}
	}
	file.close();
	init_kb_areas(keyval_left, keyval_right);
}

void Params::init_kb_areas(const KeyvalList &area_left, const KeyvalList &area_right) {

	// Chargement des listes de touches gauches et droites
	key_area[0] = aux_init_kb_areas(area_left );
	key_area[1] = aux_init_kb_areas(area_right);

	// On vérifie que l'intersection des deux ensembles est vide
	std::set<Keycode> area_both;
	area_both.insert(key_area[0].begin(), key_area[0].end());
	area_both.insert(key_area[1].begin(), key_area[1].end());
	assert(area_both.size()==key_area[0].size()+key_area[1].size());
}

std::set<Keycode> Params::aux_init_kb_areas(const KeyvalList &src) {
	std::set<Keycode> retval;
	for(KeyvalList::const_iterator it=src.begin(); it!=src.end(); ++it) {
		KeycodeList keycodes = keyval_to_keycodes(*it);
		retval.insert(keycodes.begin(), keycodes.end());
	}
	return retval;
}

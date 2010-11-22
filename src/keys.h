/******************************************************************************
 *                                                                            *
 *    This file is part of Virtual Chess Clock, a chess clock software        *
 *                                                                            *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>          *
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


#ifndef KEYS_H
#define KEYS_H

#include <gdk/gdk.h>
#include <vector>
#include <list>
#include <string>
#include <glibmm/ustring.h>


////////////////////////////////////////////////////////////////////////////////
// Types


// Types de base
typedef guint Keycode    ;
typedef guint Keyval     ;
typedef guint KeyGroup   ;
typedef guint KeyLevel   ;


// Keyval étendu
struct KeyvalEx {
	Keyval   keyval;
	KeyGroup group ;
	KeyLevel level ;
	KeyvalEx();
	KeyvalEx(Keyval kv, KeyGroup kg, KeyLevel kl);
};


// Touche de clavier "physique"
class PhysicalKey {

public:

	// Constructeur
	PhysicalKey();

	// Accesseurs
	int      bottom_line  ()        const;
	bool     in_kp        ()        const;
	int      nb_lines     ()        const;
	int      nb_keyvals   ()        const;
	int      pos_on_line  (int idx) const;
	int      width_on_line(int idx) const;
	Keyval   keyval       (int idx) const;
	KeyGroup group        (int idx) const;
	KeyLevel level        (int idx) const;
	bool     print        (int idx) const;

	// Modifieurs
	void set_bottom_line(int  src);
	void set_in_kp      (bool src);
	void set_nb_lines   (int  src);
	void set_nb_keyvals (int  src);
	void set_geometry(int idx, int pos, int width);
	void set_keyval  (int idx, Keyval keyval, KeyGroup group, KeyLevel level, bool print);


private:

	// Géométrie d'une touche
	typedef struct {
		int pos  ;
		int width;
	} KeyGeometry;

	// Valeurs associées à une touche
	typedef struct {
		Keyval   keyval;
		KeyGroup group ;
		KeyLevel level ;
		bool     print ;
	} KeyValEx;

	// Données membres
	int                      m_bottom_line;
	bool                     m_in_kp      ;
	std::vector<KeyGeometry> m_geometry   ;
	std::vector<KeyValEx   > m_keyval_ex  ;
};


// Containers
typedef std::list  <Keyval     > KeyvalList       ;
typedef std::list  <Keycode    > KeycodeList      ;
typedef std::list  <KeyvalEx   > KeyvalExList     ;
typedef std::vector<PhysicalKey> PhysicalKeyVector;



////////////////////////////////////////////////////////////////////////////////
// Fonctions algo

// Récupération du ou des keycodes à partir du keyval
KeycodeList keyval_to_keycodes(Keyval val);

// Conversions keycode vers liste de keyvals
KeyvalExList keycode_to_keyvals(Keycode code);

// Nom associé à la keyval
std::string keyval_to_string(Keyval val);

// Symbole associé à la keyval
Glib::ustring keyval_to_symbol(Keyval val);


#endif

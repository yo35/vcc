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


#ifndef DATAFILEIN_H
#define DATAFILEIN_H

#include <string>
#include <list>
#include <fstream>

class DataFileIn {

public:

	// Constructeur et routines d'ouverture
	DataFileIn(const std::string &path);
	~DataFileIn();
	void open ();
	void close();

	// Accesseurs
	const std::string &path   () const;
	bool               is_open() const;

	// Lecture
	bool eof();
	int  get();

private:

	// Sous-routines de traitement
	void parse_line();
	void check_consistency(bool test) const;

	// Données membres
	std::string    m_path;
	std::ifstream  m_file;
	int            m_curr_no_line;
	bool           m_eof;
	std::list<int> m_buff;
};

#endif

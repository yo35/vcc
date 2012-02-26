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


#ifndef DATAFILE_H
#define DATAFILE_H

#include <string>
#include <list>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// Classe de base

class DataFileBase {

public:

	// Constructeur et routines d'ouverture
	DataFileBase(const std::string &path);
	virtual void open   ()      =0;
	virtual void close  ()      =0;
	virtual bool is_open() const=0;

	// Accesseurs
	const std::string &path() const;


private:

	// Données membres
	std::string m_path;
};



////////////////////////////////////////////////////////////////////////////////
// Classe de lecture

class DataFileIn : public DataFileBase {

public:

	// Constructeur et routines d'ouverture
	DataFileIn(const std::string &path);
	virtual void open   ()      ;
	virtual void close  ()      ;
	virtual bool is_open() const;

	// Lecture
	bool eof() const;
	int  get()      ;

private:

	// Sous-routines de traitement
	void parse_line();
	void check_consistency(bool test) const;

	// Données membres
	std::ifstream  m_file;
	int            m_curr_no_line;
	bool           m_eof;
	std::list<int> m_buff;
};



////////////////////////////////////////////////////////////////////////////////
// Classe d'écriture

class DataFileOut : public DataFileBase {

public:

	// Constructeur et routines d'ouverture
	DataFileOut(const std::string &path);
	virtual void open   ()      ;
	virtual void close  ()      ;
	virtual bool is_open() const;

	// Écriture
	void put(int val);

private:

	// Données membres
	std::ofstream m_file;
	int           m_cpt_items_on_line;
};

#endif

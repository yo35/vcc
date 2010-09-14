/*
 *    This file is part of Virtual Chess Clock, a chess clock software
 *
 *    Copyright (C) 2010  Yoann Le Montagner <yo35(at)melix(dot)net>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TIMER_H
#define TIMER_H

#include <glibmm/object.h>
#include <sys/time.h>

class Timer : public Glib::Object {

public:

	// Le mode du timer
	typedef enum {
		INCREMENT,
		DECREMENT,
		PAUSED
	} Mode;

	// Divers
	Timer();
	sigc::signal<void> signal_modified() const;

	// Contrôle du timer
	void set_mode(Mode new_mode);
	void set_time(int  new_time);
	Mode get_mode() const;
	int  get_time() const;

private:

	// Structure interne pour le repérage d'un instant dans le temps
	typedef struct timeval timeval_t;

	// Routines internes
	void refresh_time() const;
	bool on_timeout_elapses();
	static int difftime(const timeval_t &t2, const timeval_t &t1);

	// Données membres
	Mode               m_mode;
	int                m_time;
	timeval_t          m_start_at;
	sigc::signal<void> m_signal_modified;
};

#endif

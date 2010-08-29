
#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <glibmm/object.h>

class Timer : public Glib::Object {

public:

	// Le mode du timer
	typedef enum {
		INCREMENT,
		DECREMENT,
		PAUSED
	} Mode;

	// Constructeur
	Timer();

	void set_mode(Mode new_mode);
	Mode get_mode() const;

	void set_time(int new_time);
	int  get_time() const;

protected:

	// Routine a surcharger pour la gestion de l'affichage
	virtual void refresh_widget() = 0;
	int get_raw_time() const;

private:

	// Routines internes
	void refresh_time() const;
	bool on_timeout_elapses();
	void reset_counter();

	// Données membres
	Mode   mode;
	mutable int curr_time;
	time_t start_at;
	int    time_at_begin;
};

#endif

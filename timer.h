
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

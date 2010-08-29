
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

	// Constructeur
	Timer();

	sigc::signal<void> signal_modified() const;

	void set_mode(Mode new_mode);
	Mode get_mode() const;

	void set_time(int new_time);
	int  get_time() const;


private:

	// Routines internes
	void refresh_time() const;
	bool on_timeout_elapses();
	static int difftime(const struct timeval &t2, const struct timeval &t1);

	// Données membres
	Mode               m_mode;
	int                m_time;
	struct timeval     m_start_at;
	sigc::signal<void> m_signal_modified;
};

#endif

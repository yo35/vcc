
#ifndef DIGITALTIMER_H
#define DIGITALTIMER_H

#include <gtkmm/drawingarea.h>
#include "timer.h"

class DigitalTimer : public Gtk::DrawingArea {

public:

	// Constructeur
	DigitalTimer();
	void set_timer(const Timer &timer);

protected:

	virtual bool on_expose_event(GdkEventExpose *event);

private:

	// Routines internes
	void refresh_widget();

	// Données membres
	const Timer *m_timer;
};

#endif

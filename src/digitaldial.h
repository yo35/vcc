
#ifndef DIAL_H
#define DIAL_H

#include <gtkmm/drawingarea.h>
#include "timer.h"

class Dial : public Gtk::DrawingArea {

public:

	// Divers
	Dial();
	void set_timer(const Timer &timer);

protected:

	// Routine de dessin
	virtual bool on_expose_event(GdkEventExpose *event);

private:

	// Force le redessin du widget
	void refresh_widget();

	// Données membres
	const Timer *m_timer;
};

#endif

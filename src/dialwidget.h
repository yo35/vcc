
#ifndef DIALWIDGET_H
#define DIALWIDGET_H

#include <gtkmm/drawingarea.h>
#include "timer.h"

class DialWidget : public Gtk::DrawingArea {

public:

	// Divers
	DialWidget();
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


#ifndef DIGITALTIMER_H
#define DIGITALTIMER_H

#include <gtkmm/drawingarea.h>
#include "abstracttimer.h"

class DigitalTimer : public AbstractTimer, public Gtk::DrawingArea {

public:

	// Constructeur
	DigitalTimer();

protected:
	
	virtual void refresh_widget();
	virtual bool on_expose_event(GdkEventExpose *event);
};

#endif

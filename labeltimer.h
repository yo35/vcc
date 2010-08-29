
#ifndef LABELTIMER_H
#define LABELTIMER_H

#include <gtkmm/label.h>
#include "abstracttimer.h"

class LabelTimer : public AbstractTimer, public Gtk::Label {

public:

	// Constructeur
	LabelTimer();

protected:
	
	virtual void refresh_widget();
};

#endif

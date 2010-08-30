
#ifndef TIMECONTROL_H
#define TIMECONTROL_H

class TimeControl {

public:

	// 4 modes de contrôle de temps sont disponibles
	typedef enum {
		SIMPLE_DELAY,
		FISCHER,
		BRONSTEIN,
		HOUR_GLASS
	} Mode;

	// Constructeur
	TimeControl();

	// Accesseurs
	Mode mode() const;
	int  main_time(int no) const;
	int  increment(int no) const;

	// Modifieurs
	void set_mode(Mode new_mode);
	void set_main_time(int new_main_time, int no=-1);
	void set_increment(int new_increment, int no=-1);

private:

	// Données membres
	Mode m_mode;
	int  m_main_time[2];
	int  m_increment[2];
};

#endif

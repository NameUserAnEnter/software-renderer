#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>

struct Clock {
	static void TimeUpdate();
	static clock_t seconds_elapsed;
	static unsigned int updates_in_current_sec;
	static unsigned int updates_in_last_sec;
};

#endif


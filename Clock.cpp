#include "Clock.h"

clock_t Clock::seconds_elapsed = 0;
unsigned int Clock::updates_in_current_sec = 0;
unsigned int Clock::updates_in_last_sec = 0;

void Clock::TimeUpdate() {
	clock_t update_entry_time = clock();
	updates_in_current_sec++;

	if (update_entry_time / CLOCKS_PER_SEC > seconds_elapsed) {
		updates_in_last_sec = updates_in_current_sec;
		updates_in_current_sec = 0;

		seconds_elapsed = update_entry_time / CLOCKS_PER_SEC;
	}
}


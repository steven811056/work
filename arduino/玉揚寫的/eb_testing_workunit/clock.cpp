// 
// 
// 

#include "clock.h"

ClockClass Clock;

void ClockClass::update()
{
	unsigned long new_micros = micros();
	unsigned long delta;
	if (new_micros < time_micros) {
		delta = (~(0UL) - time_micros) + 1 + new_micros;
	}
	else {
		delta = new_micros - time_micros;
	}
	time_micros = new_micros;

	time += delta;

	if (time > 1800000000UL) {
		time -= 1800000000UL;
	}
}

long ClockClass::pass(int32_t last)
{
	if (last > time) {
		last -= 1800000000UL;
	}
	return time - last;
}

bool ClockClass::pass(int32_t & last, int32_t thr)
{
	if (thr < 1) {
		return 0;
	}
	if (last > time) {
		last -= 1800000000UL;
	}
	if (last + thr < time) {
		last = time;
		return true;
	}
	return false;
}

uint16_t ClockClass::pass_t(int32_t & last, int32_t uint)
{
	if (uint < 1) {
		return 0;
	}
	uint16_t res = 0;
	if (last > time) {
		last -= 1800000000UL;
	}
	long dur = time - last;
	if (uint < dur) {
		res = dur / uint;
		last += res * uint;
	}
	return res;
}

long ClockClass::get()
{
	return time;
}

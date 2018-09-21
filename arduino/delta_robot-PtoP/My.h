#ifndef My_h
#define My_h

#include "arduino.h"

class MyDelta
{
public:
	MyDelta(double tt);
	double _hh;
	void return1();
	const int MstepPin[3] = { 2,4,6 };
	const int MdirPin[3] = { 3,5,7 };
	const int Msenser[3] = { 8,9,10 };
	const int Mdelaytime = 1;

};

#endif


#ifndef return_h
#define return_h

#include "arduino.h"


class DeltaReturn
{
public:
	DeltaReturn();
	int senser[3] = { 8,9,10 };
	int stepPin[3] = { 2,4,6 };
	int dirPin[3] = { 3,5,7 };	
	int delaytime = 10;

};

extern DeltaReturn DeltaRE;


#endif

#ifndef Stop_h
#define Stop_h
#include <arduino.h>

class EStop
{
public:	
	EStop(void);
	char Scara_Stop;
	int EMStop();
	void EMStop2();

};

#endif
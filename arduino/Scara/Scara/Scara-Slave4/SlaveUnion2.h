#ifndef SlaveUnion2_h
#define SlaveUnion2_h
#include "arduino.h"
#include <stdint.h>

class UnionTurn2
{
	
public:	
	int32_t incommingByte = 0;
	int testT[8];
	int a;
	UnionTurn2(void);
	void CommingAdd(void);
	void Start(void);
	int END(void);

};

#endif
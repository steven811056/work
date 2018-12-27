#ifndef SlaveUnion_h
#define SlaveUnion_h
#include "Arduino.h"


class UnionTurn
{

public:	
	int incommingByte = 0;
	int testT[4];

	UnionTurn(void);
	void CommingAdd(void);
	void Start(void);
	int END(void);

};

#endif
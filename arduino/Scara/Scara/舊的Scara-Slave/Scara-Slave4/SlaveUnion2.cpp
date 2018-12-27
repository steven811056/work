#include "SlaveUnion2.h"
#include "Arduino.h"
#include <Wire.h>


UnionTurn2::UnionTurn2(void)
{
	Start();	
}

void UnionTurn2::Start(void)
{
	for (int i = 0; i < 8; i++)
	{
		testT[i] = 0;
	}
	Serial.println("Wire.available");
	int i = 0;	
	while (Wire.available())
	{		
		testT[i] = Wire.read();
		Serial.print("i = ");
		Serial.println(i);
		Serial.println(testT[i]);
		i++;
	}
	CommingAdd();
}

void UnionTurn2::CommingAdd()
{
	Serial.println("Add");
	incommingByte = 0;
	a = 0;
	for (int i = 0; i < 8; i++)
	{
		if (testT[i] < 0)
		{
			break;
		}
		Serial.println(testT[i]);
		if (i == 0)
		{
			incommingByte = incommingByte + testT[i];
		}
		if (testT[i] == 0)
		{
			break;
		}		
		a = testT[i] * pow(256, i);
		if (a == testT[i])
		{
			a = a - testT[i];
			a++;
		}
		if ((a - testT[i] * pow(256, i)) > 0)
		{
			a++;
		}
		incommingByte = incommingByte +a;
		Serial.println(incommingByte);		
	}	
	Serial.println("end");
	END();
}

int UnionTurn2::END(void)
{		
	return 1;
}
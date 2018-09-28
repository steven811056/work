#include "C:\Users\USER\Desktop\arduino-1.8.3 - bootloader\libraries\SlaveUnion\SlaveUnion.h"
#include "Arduino.h"
#include <Wire.h>


UnionTurn::UnionTurn(void)
{
	Start();
	for (int i = 0; i < 4; i++)
	{
		testT[i] = 0;
	}
}

void UnionTurn::Start(void)
{
	Serial.println("Wire.available");
	int i = 0;	
	while (Wire.available())
	{
		//Serial.println("123");
		testT[i] = Wire.read();
		Serial.print("i = ");
		Serial.println(i);
		Serial.println(testT[i]);
		i++;
	}
	CommingAdd();
}

void UnionTurn::CommingAdd()
{
	Serial.println("Add");
	incommingByte = 0;
	for (int i = 0; i < 4; i++)
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
		incommingByte = incommingByte + testT[i] * (i * 256);
		Serial.println(incommingByte);
	}
	Serial.println(incommingByte);	
	END();
}

int UnionTurn::END(void)
{
	return 1;
}
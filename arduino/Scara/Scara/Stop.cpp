#include "C:\steven-git\work\arduino\Scara\Scara\Stop.h"
#include <arduino.h>

EStop::EStop(void)
{
	
};

int EStop::EMStop()
{
	Serial.println("Stop.h");
	if (Serial.available())
	{
		Serial.println("Stop.h  EMStop");
		Scara_Stop = Serial.read();
		if (Scara_Stop == 'P' || Scara_Stop == 'p')
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
};

void EStop::EMStop2()
{
	Serial.println("Stop.h");
	if (Serial.available())
	{
		Serial.println("Stop.h  EMStop");
		Scara_Stop = Serial.read();
		if (Scara_Stop == 'P' || Scara_Stop == 'p')
		{
			Serial.println("Stop.h  EMStop   1233");
		}
	}
}
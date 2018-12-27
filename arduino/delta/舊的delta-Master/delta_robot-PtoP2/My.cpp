#include "Arduino.h"
#include "My.h"
#include <math.h>

MyDelta::MyDelta(double tt)
{
	_hh = tt;

}

void MyDelta::return1()
{
	Serial.println("歸零啟動");
	Serial.println((int)digitalRead(8));
	Serial.println(digitalRead(Msenser[1]));
	Serial.println(digitalRead(Msenser[2]));
	while (digitalRead(Msenser[0]) == HIGH || digitalRead(Msenser[1]) == HIGH || digitalRead(Msenser[2]) == HIGH)
	{
		Serial.println("歸零啟動1");
		digitalWrite(MdirPin[0], HIGH);
		digitalWrite(MdirPin[1], HIGH);
		digitalWrite(MdirPin[2], HIGH);
		delay(30);
		if (digitalRead(Msenser[0]) == HIGH)
		{
			digitalWrite(MstepPin[0], HIGH);
			delayMicroseconds(Mdelaytime);
			digitalWrite(MstepPin[0], LOW);
			delayMicroseconds(Mdelaytime);

		}
		else
		{
			Serial.println("Msenser[0]  OK");

		}
		if (digitalRead(Msenser[1]) == HIGH)
		{
			digitalWrite(MstepPin[1], HIGH);
			delayMicroseconds(Mdelaytime);
			digitalWrite(MstepPin[1], LOW);
			delayMicroseconds(Mdelaytime);

		}
		else
		{
			Serial.println("Msenser[1]  OK");

		}
		if (digitalRead(Msenser[2]) == HIGH)
		{
			digitalWrite(MstepPin[2], HIGH);
			delayMicroseconds(Mdelaytime);
			digitalWrite(MstepPin[2], LOW);
			delayMicroseconds(Mdelaytime);

		}
		else
		{
			Serial.println("Msenser[2]  OK");

		}

	}

}
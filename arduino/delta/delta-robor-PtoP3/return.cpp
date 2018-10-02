#include "Arduino.h"
#include "return.h"
#include <math.h>

DeltaReturn DeltaRE;

DeltaReturn::DeltaReturn()		//Âk¹s¨ç¦¡
{
	for (int i = 0; i < 3; i++)
	{
		pinMode(senser[i], INPUT);		

	}
	Serial.println("Âk¹s±Ò°Ê");
	Serial.println((int)digitalRead(8));
	Serial.println(digitalRead(senser[1]));
	Serial.println(digitalRead(senser[2]));	
	while (1)
	{
		digitalWrite(dirPin[0], HIGH);
		digitalWrite(dirPin[1], HIGH);
		digitalWrite(dirPin[2], HIGH);

		if (digitalRead(senser[0]) == HIGH)
		{
			digitalWrite(stepPin[0], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[0], LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[0]  start");

		}
		if (digitalRead(senser[0]) == LOW)
		{
			Serial.println("senser[0]  OK");

		}

		if (digitalRead(senser[1]) == HIGH)
		{
			digitalWrite(stepPin[1], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[1], LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[1]  start");

		}
		if (digitalRead(senser[1]) == LOW)
		{
			Serial.println("senser[1]  OK");

		}

		if (digitalRead(senser[2]) == HIGH)
		{
			digitalWrite(stepPin[2], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[2], LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[2]  start");

		}
		if (digitalRead(senser[2]) == LOW)
		{
			Serial.println("senser[2]  OK");

		}
		if (digitalRead(senser[0]) == LOW && digitalRead(senser[1]) == LOW && digitalRead(senser[2]) == LOW)
		{
			break;

		}

	}

}
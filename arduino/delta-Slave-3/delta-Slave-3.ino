#include <Wire.h>
const int SLAVE_ADDRESS = 3;
char incomingByte = '0';
String incomingString = "";

const int stepPin[3] = { 2,4,6 };
const int dirPin[3] = { 3,5,7 };
const int senser[3] = { 8,9,10 };
const int delaytime = 300;
boolean debug = 1;
int incomingIntShow = 30;

void setup()
{
	Wire.begin(SLAVE_ADDRESS);
	Wire.onReceive(test);
	Serial.begin(9600);
	if (debug)
	{
		Serial.println("--> Slave 3 Ready");
	}
	for (int i = 0; i < 3; i++)
	{
		pinMode(senser[i + 8], INPUT);
		pinMode(stepPin[i], OUTPUT);
		pinMode(dirPin[i], OUTPUT);
	}

}

void loop()
{
	/*return1();
	delay(1200);
	turn1();
	delay(1200);
	return1();
	delay(1200);*/

}

void test(int t)
{
	if (debug)
	{
		Serial.println("Slave 3 receive get");
	}
	while (Wire.available())
	{
		incomingByte = (char)Wire.read();
		incomingString = incomingString + incomingByte;
		if (debug)
		{
			Serial.println(incomingByte);
		}

	}
	if (incomingString == "return0")
	{
		Wire.onReceive(return0);
	}
	if (incomingString == "start")
	{
		Wire.onReceive(Turn);
	}
}

void Turn(int t)
{
	int incomingInt;
	if (debug)
	{
		Serial.println("--> Turn start");
	}
	incomingString = "";
	while (Wire.available())
	{
		incomingInt = (int)Wire.read();

		if (debug)
		{
			Serial.println(incomingInt / 0.1125);
		}
	}
	digitalWrite(dirPin[2], LOW);
	for (int i = (incomingInt/0.225); i > 0; i = i - 1)
	{
		digitalWrite(stepPin[2], HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepPin[2], LOW);
		delayMicroseconds(delaytime);
		Serial.print("-->");
		Serial.println(i);
	}
	Wire.onReceive(test);

}

void return0(int t)
{
	incomingString = "";
	while (Wire.available())
	{
		incomingByte = (char)Wire.read();
		incomingString = incomingString + incomingByte;
		if (debug)
		{
			Serial.println(incomingByte);
		}

	}
	if (incomingString == "start")
	{
		if (debug)
		{
			Serial.println("frequency send");
		}
		while (incomingString == "start")
		{
			digitalWrite(dirPin[2], HIGH);
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
				Wire.onReceive(test);
				incomingString = "";
				break;

			}
		}
	}

}

void turn1()
{
	digitalWrite(dirPin[2], LOW);
	for (int i = (incomingIntShow / 0.225); i > 0; i = i - 1)
	{
		digitalWrite(stepPin[2], HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepPin[2], LOW);
		delayMicroseconds(delaytime);
		Serial.print("-->");
		Serial.println(i);
	}
	Wire.onReceive(test);
}

void return1()
{
	while (1)
	{
		digitalWrite(dirPin[2], HIGH);
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
			Wire.onReceive(test);
			incomingString = "";


			break;
		}

	}

}

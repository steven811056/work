#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

#define turndegree 33

const int SLAVE_ADDRESS = 0x04;
char incomingByte = '0';
String incomingString = "";
int dirPin = A0;
int stepperPin = A1;
int suck = 3;
const int senser = 2;
uint32_t incomingInt = 0;
boolean debug = 1;
volatile int Show = 0;
int delaytime = 40;

UnionTurn testU;

void setup()
{
	Serial.begin(9600);
	Wire.begin(SLAVE_ADDRESS);
	Wire.onReceive(test);
	pinMode(A2, OUTPUT);
	if (debug)
	{
		Serial.println("--> Slave 4 Ready");
	}
	pinMode(stepperPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(senser, INPUT);
	pinMode(suck, OUTPUT);
	pinMode(A2, OUTPUT);
	digitalWrite(senser, LOW);
}

void loop()
{
	digitalWrite(A2, LOW);
	if (Show == 1)
	{
		if (debug)
		{
			Serial.println("OPEN");
		}
		digitalWrite(suck, HIGH);
		Show = 0;
	}
	else if (Show == 2)
	{
		if (debug)
		{
			Serial.println("CLOSE");
		}
		digitalWrite(suck, LOW);
		Show = 0;
	}
	else;
}

void test(int t)
{
	if (debug)
	{
		Serial.println("Slave 4 receive get");
	}
	incomingString = "";
	while (Wire.available())
	{
		incomingByte = (char)Wire.read();
		incomingString = incomingString + incomingByte;
		if (debug)
		{
			Serial.println(incomingString);
		}
	}
	if (incomingString == "open")
	{
		Show = 1;
	}
	if (incomingString == "close")
	{
		Show = 2;
	}
	if (incomingString == "turn")
	{
		Show = 3;
	}
}




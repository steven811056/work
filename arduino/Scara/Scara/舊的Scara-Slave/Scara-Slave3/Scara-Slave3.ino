#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

const int SLAVE_ADDRESS = 0x03;
char incomingByte = '0';
String incomingString = "";

int dirPin = A0;
int stepperPin = A1;
const int senser = 2;
uint32_t incomingInt = 0;
const int delaytime = 40;
boolean debug = 1;
int Show = 0;

UnionTurn testU;

void setup()
{
	Wire.begin(SLAVE_ADDRESS);
	Wire.onReceive(test);
	Serial.begin(9600);
	pinMode(A2, OUTPUT);
	if (debug)
	{
		Serial.println("--> Slave 3 Ready");
	}
	pinMode(senser, INPUT);
	pinMode(stepperPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(A2, OUTPUT);
	digitalWrite(senser, LOW);
}

void loop()
{
	if (incomingString == "reset")
	{
		Serial.println("reset");
		return1();
		Serial.println("reset  END ");
	}
	if (Show == 1)
	{
		Turn();
	}
	if (Show == 2)
	{
		Turn2();
	}
}

void test(int t)
{
	if (debug)
	{
		Serial.println("Slave 3 receive get");
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
	if (incomingString == "start")
	{
		Wire.onReceive(testUU);
	}
	if (incomingString == "start2")
	{
		Wire.onReceive(testUZ);
	}
}

void testUU(int a)
{
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 1;
	}
	if (testU.END() == 2)
	{
		Serial.println("--> Nothing ");
	}
}

void testUZ(int a)
{
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 2;
	}
	if (testU.END() == 2)
	{
		Serial.println("--> Nothing ");
	}
}

void Turn()
{
	if (debug)
	{
		Serial.println("--> Turn1 start");
	}
	incomingInt = testU.incommingByte;
	Serial.println(incomingInt);
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, HIGH);
	//int32_t  i = (incomingInt * 20) /( 0.05625*2);
	uint32_t i = incomingInt;
	Serial.println(i);
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
		/*Serial.print("-->");
		Serial.println(i);*/
	}
	/*if (Show == 1)
	{
		Serial.println("delay---");
		delay(1000);
		Turn2();
	}
	if (Show == 2)
	{
		incomingInt = 0;
		Show = 0;
		Wire.onReceive(test);
	}*/
	Show = 0;
	Wire.onReceive(test);
}

void Turn2()
{
	if (debug)
	{
		Serial.println("--> Turn2 start");
	}
	incomingInt = testU.incommingByte;
	Serial.println(incomingInt);
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, LOW);
	//uint32_t i = (incomingInt * 20) / (0.05625 * 2);
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
		/*Serial.print("-->");
		Serial.println(i);*/
	}
	/*if (Show == 1)
	{
		incomingInt = 0;
		Show = 0;
		Wire.onReceive(test);
	}
	if (Show == 2)
	{
		Serial.println("delay---");
		delay(1000);
		Turn();
	}*/
	Show = 0;
	Wire.onReceive(test);
}

void return1()
{
	incomingString = "";
	digitalWrite(dirPin, HIGH);
	uint32_t i = (110 * 10) / 0.05625;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}
	delay(1000);
	while (1)
	{
		digitalWrite(dirPin, LOW);
		if (digitalRead(senser) == HIGH)
		{			
			digitalWrite(stepperPin, HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepperPin, LOW);
			delayMicroseconds(delaytime);
		}
		if (digitalRead(senser) == LOW)
		{
			Serial.println("senser  OK");
			incomingString = "";			
			break;
		}
	}
	Wire.onReceive(test);
}
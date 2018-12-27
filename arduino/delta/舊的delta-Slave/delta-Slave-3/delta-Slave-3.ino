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
const int delaytime = 250;
int reset_delaytime = 40;
int8_t data[2];
boolean debug = 1;
int incomingIntShow = 100;
int Show = 0;
int suck = 3;

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
	pinMode(suck, OUTPUT);
}

UnionTurn testU;

void loop()
{
	if (incomingString == "reset")
	{
		Serial.println("reset");
		return1();
		Serial.println("reset  END ");
	}
	else if (incomingString == "start")
	{
		Wire.onReceive(testUU);
	}
	else if (incomingString == "start2")
	{
		Wire.onReceive(testUZ);
	}
	else if (incomingString == "open")
	{
		Show = 3;
	}
	else if (incomingString == "close")
	{
		Show = 4;
	}
	else;
	if (Show == 1)
	{
		Turn();
	}
	else if (Show == 2)
	{
		Turn2();
	}
	else if (Show == 3)
	{
		if (debug)
		{
			Serial.println("OPEN");
		}
		digitalWrite(suck, HIGH);
		incomingString = "";
		Show = 0;
	}
	else if (Show == 4)
	{
		if (debug)
		{
			Serial.println("CLOSE");
		}
		digitalWrite(suck, LOW);
		incomingString = "";
		Show = 0;
	}
	else;
}

void test(int t)
{
	if (debug)
	{
		Serial.println("Slave 3 receive get");
	}
	else;	
	while (Wire.available())
	 {
		incomingByte = (char)Wire.read();
		incomingString = incomingString + incomingByte;
		if (debug)
		{
			Serial.println(incomingString);
		}
	 }	
}

void testUU(int a)
{
	incomingString = "";
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 1;
	}
}

void testUZ(int a)
{
	incomingString = "";
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 2;
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
	}
	Show = 0;
	incomingString = "";
	Wire.onReceive(test);
	Serial.println(" --> Turn1 END");
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
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);	
	}	
	Show = 0;
	incomingString = "";
	Wire.onReceive(test);
	Serial.println(" --> Turn2 END");
}

void return1()
{
	incomingString = "";
	while (1)
	{
		digitalWrite(dirPin, HIGH);
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
}
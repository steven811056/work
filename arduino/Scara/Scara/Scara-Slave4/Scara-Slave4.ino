#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>
#include "ByteBuffer.h"

#define turndegree 33

const int SLAVE_ADDRESS = 0x04;
char incomingByte = '0';
String incomingString = "";
int dirPin = A0;
int stepperPin = A1;
int suck = 3;
const int senser = 2;
int32_t incomingInt = 0;
boolean debug = 1;
volatile int Show = 0;
int delaytime = 40;

ByteBuffer wireBuf(32);

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
	digitalWrite(A2, LOW);
}

UnionTurn testU;
void loop()
{		
	if (incomingString == "open")
	{
		Show = 1;
	}
	else if (incomingString == "close")
	{
		Show = 2;
	}
	else if (incomingString == "turn")
	{	
		reset_String();
		Show = 3;
	}	
	else if (Show == 3)
	{
		if (incomingString == "start")
		{
			Wire.onReceive(turn);			
		}
		else if (incomingString == "start2")
		{
			Wire.onReceive(back_turn);
		}
	}
	else;
	if (Show == 1)
	{
		if (debug)
		{
			Serial.println("OPEN");
		}
		digitalWrite(suck , HIGH);
		Show = 0;
		reset_String();
	}
	else if (Show == 2)
	{
		if (debug)
		{
			Serial.println("CLOSE");
		}
		digitalWrite(suck, LOW);
		Show = 0;
		reset_String();
	}
	else if (Show == 3)
	{	
		Serial.println("Show == 3");		
	}
	else if (Show == 4)
	{
		if (debug)
		{
			Serial.println("TURN");
		}
		suck_run_Forward();
		Wire.onReceive(test);
		Show = 0;
		reset_String();		
	}
	else if (Show == 5)
	{
		if (debug)
		{
			Serial.println("TURN2");
		}
		suck_run_backward();
		Wire.onReceive(test);
		Show = 0;
		reset_String();	}
	else;
}
// ----- I2C呼叫方程式
void test(int t)
{
	if (debug)
	{
		Serial.println("Slave 4 receive get");
	}
	if (Show == 0 || Show ==3)
	{
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
}

void turn(int a)
{	
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 4;
	}
}

void back_turn(int a)
{
	testU.Start();
	if (testU.END() == 1)
	{
		Show = 5;
	}
}
// ----- ******   I2C呼叫方程式

// ----- 轉盤正 OR 反轉
void suck_run_Forward()
{	
	Serial.println("forward");
	digitalWrite(dirPin, HIGH);
	incomingInt = testU.incommingByte;
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}	
}

void suck_run_backward()
{
	Serial.println("backward");
	digitalWrite(dirPin, LOW);
	incomingInt = testU.incommingByte;
	uint32_t i = incomingInt;
	for (i; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}	
}
// -----  **** 轉盤正 OR 反轉
void reset_String()
{
	Serial.println("reset_String");
	incomingString = "";
}




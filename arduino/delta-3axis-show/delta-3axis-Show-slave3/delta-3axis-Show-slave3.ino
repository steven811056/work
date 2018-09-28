#include <Wire.h>
#include "C:\Users\USER\Desktop\arduino-1.8.3 - bootloader\libraries\SlaveUnion\SlaveUnion.h"

const int SLAVE_ADDRESS = 0x03;
char incomingByte = '0';
String incomingString = "";

int dirPin = A0;
int stepperPin = A1;
const int senser = 2 ;
int incomingInt =0;
const int delaytime = 20;
int8_t data[2];
boolean debug = 1;
int incomingIntShow = 100;
int Show;

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
	for (int i = 0; i < 3; i++)
	{
		pinMode(senser, INPUT);
		pinMode(stepperPin, OUTPUT);
		pinMode(dirPin, OUTPUT);
	}
	pinMode(A2, OUTPUT);
}

void loop()
{
	if (digitalRead(senser) == HIGH)
	{
		Serial.println("senser  HIGH");
		digitalWrite(A2, LOW);
		digitalWrite(dirPin, HIGH);
		uint32_t  i = 20000;
		Serial.println(i);
		for (i; i > 0; i = i - 1)
		{
			digitalWrite(stepperPin, HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepperPin, LOW);
			delayMicroseconds(delaytime);
		}
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
	if (incomingString == "return0")
	{
		Wire.onReceive(return0);
	}
	if (incomingString == "start")
	{
		Wire.onReceive(testUU);
	}
}

void requestEvent()
{
	Serial.println(data[0]);
	Wire.write(data[0]);

}

void testUU(int a)
{
	testU.Start();
	if (testU.END() == 1)
	{
		Turn(1);
	}
}

void Turn(int t)
{
	if (debug)
	{
		Serial.println("--> Turn start");
	}	
	incomingInt = testU.incommingByte ;
	Serial.println(incomingInt);	
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, HIGH);	
	uint32_t  i = (incomingInt * 20) / 0.225;
	Serial.println(i);
	for ( i ; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
		/*Serial.print("-->");
		Serial.println(i);*/
	}	
	delay(5500);
	Turn2(1);

}

void Turn2(int t)
{
	if (debug)
	{
		Serial.println("--> Turn start");
	}		
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, LOW);	
	uint32_t i = (incomingInt * 20) / 0.225  ;
	for ( i ; i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
		/*Serial.print("-->");
		Serial.println(i);*/
	}
	incomingInt = 0;
	Wire.onReceive(test);

}

void return0(int t)
{
	incomingString = "";
	while (Wire.available())
	{
		incomingByte = (int)Wire.read();
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
			digitalWrite(dirPin, HIGH);
			if (digitalRead(senser) == HIGH)
			{
				digitalWrite(stepperPin, HIGH);
				delayMicroseconds(delaytime);
				digitalWrite(stepperPin, LOW);
				delayMicroseconds(delaytime);
				Serial.println("senser  start");

			}
			if (digitalRead(senser) == LOW)
			{
				Serial.println("senser  OK");
				Wire.onReceive(test);
				incomingString = "";


				break;
			}
		}
	}

}


void return1()
{
	while (1)
	{
		digitalWrite(dirPin, HIGH);
		if (digitalRead(senser) == HIGH)
		{
			digitalWrite(stepperPin, HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepperPin, LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser  start");

		}
		if (digitalRead(senser) == LOW)
		{
			Serial.println("senser  OK");
			Wire.onReceive(test);
			incomingString = "";


			break;
		}

	}

}
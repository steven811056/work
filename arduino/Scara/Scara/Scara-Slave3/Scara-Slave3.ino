#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

const int SLAVE_ADDRESS = 0x03;
char incomingByte = '0';
String incomingString = "";

int dirPin = A0;
int stepperPin = A1;
const int senser = 2 ;
uint32_t incomingInt =0;
const int delaytime = 50;
int8_t data[2];
boolean debug = 1;
int incomingIntShow = 100;
int Show = 0;
uint8_t First_Senser = 0;

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
		Show = 1;
	}
}

void Turn()
{
	if (debug)
	{
		Serial.println("--> Turn start");
	}	
	incomingInt = testU.incommingByte ;
	Serial.println(incomingInt);	
	digitalWrite(A2, LOW);
	digitalWrite(dirPin, HIGH);	
	uint32_t  i =  (uint32_t)incomingInt*3200+320;
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
	delay(1000);
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
	uint32_t i = incomingInt * 3200+320;
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
	Show = 0;
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
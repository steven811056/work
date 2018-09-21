#include <Wire.h>
const int SLAVE_ADDRESS = 0x03;
char incomingByte = '0';
String incomingString = "";

//const int stepPin[3] = { 2,4,6 };
//const int dirPin[3] = { 3,5,7 };
int dirPin = 2;
int stepperPin = 3;
const int senser[3] = { 8,9,10 };
int incomingInt;
const int delaytime = 50;
int8_t data[2];
boolean debug = 1;
int incomingIntShow = 100;
int Show;

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
		pinMode(senser[i + 8], INPUT);
		pinMode(stepperPin, OUTPUT);
		pinMode(dirPin, OUTPUT);
	}
}

void loop()
{

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
		Wire.onReceive(Turn);
	}
}

void requestEvent()
{
	Serial.println(data[0]);
	Wire.write(data[0]);

}

void Turn(int t)
{
	if (debug)
	{
		Serial.println("--> Turn start");
	}
	incomingString = "";
	while (Wire.available())
	{
		incomingInt = incomingInt + Wire.read();
		Serial.println(incomingInt);

		if (debug)
		{
			Serial.println(incomingInt / 0.1125);
		}
	}
	digitalWrite(dirPin, HIGH);
	digitalWrite(A2, LOW);
	for (int i = (incomingInt / 0.225 ); i > 0; i = i - 1)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
		/*Serial.print("-->");
		Serial.println(i);*/
	}
	incomingInt = 0;
	Wire.onReceive(Turn2);

}

void Turn2(int t)
{
	if (debug)
	{
		Serial.println("--> Turn start");
	}
	incomingString = "";
	while (Wire.available())
	{
		incomingInt = incomingInt + Wire.read();
		if (debug)
		{
			Serial.println(incomingInt / 0.1125);
		}
	}
	digitalWrite(dirPin, LOW);
	digitalWrite(A2, LOW);
	for (int i = (incomingInt / 0.225 ); i > 0; i = i - 1)
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
			if (digitalRead(senser[0]) == HIGH)
			{
				digitalWrite(stepperPin, HIGH);
				delayMicroseconds(delaytime);
				digitalWrite(stepperPin, LOW);
				delayMicroseconds(delaytime);
				Serial.println("senser[0]  start");

			}
			if (digitalRead(senser[0]) == LOW)
			{
				Serial.println("senser[0]  OK");
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
		if (digitalRead(senser[0]) == HIGH)
		{
			digitalWrite(stepperPin, HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepperPin, LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[0]  start");

		}
		if (digitalRead(senser[0]) == LOW)
		{
			Serial.println("senser[0]  OK");
			Wire.onReceive(test);
			incomingString = "";


			break;
		}

	}

}
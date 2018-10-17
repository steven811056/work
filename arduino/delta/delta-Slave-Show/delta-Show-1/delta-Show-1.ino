#include <SlaveUnion.h>
#include <Wire.h>
#include <arduino.h>

const int SLAVE_ADDRESS = 0x01;
char incomingByte = '0';
String incomingString = "";

int dirPin = A0;
int stepperPin = A1;
const int senser = 2;
uint32_t incomingInt = 0;
const int delaytime = 20;
int8_t data[2];
boolean debug = 1;
int incomingIntShow = 100;
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
		Serial.println("--> Slave 1 Ready");
	}
	pinMode(senser, INPUT);
	pinMode(stepperPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(A2, OUTPUT);
	digitalWrite(senser, LOW);
	digitalWrite(A2, LOW);
}

void loop()
{
	if (incomingString == "reset")
	{
		Serial.println("reset");		
		Serial.println("reset  END ");
	}
	
}

void test(int t)
{
	if (debug)
	{
		Serial.println("Slave 1 receive get");
	}
	incomingString = "";
	while (Wire.available())
	{
		incomingByte = Wire.read();
		if (incomingByte == 1)
		{
			Start(50);
		}
	}	
}

void Start(int x)
{	
	digitalWrite(dirPin, HIGH);
	uint32_t  i = x / (0.05625 );
	for (int i;i>0;i--)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);	
	}

	digitalWrite(dirPin, LOW);
	uint32_t  i = x / (0.05625);
	for (int i; i>0; i--)
	{
		digitalWrite(stepperPin, HIGH);
		delayMicroseconds(delaytime);
		digitalWrite(stepperPin, LOW);
		delayMicroseconds(delaytime);
	}

}
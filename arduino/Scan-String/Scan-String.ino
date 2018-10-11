#include "math.h"
#include <arduino.h>
int i = 0;
int ST = 0;
char ReceiveString[200] ;

void setup()
{
	Serial.begin(9600);
	for (int i = 0; i < sizeof(ReceiveString); i++)
	{
		ReceiveString[i] = ' ';
	}
}

void loop() 
{	
	while (Serial.available())
	{
		ST = Serial.read();
		i++;
		Serial.print(ST);
	}	
	
	/*for (int k = 0; k < i; k++)
	{
		ST = ReceiveString[i];
		Serial.print(ST);
	}*/
	Serial.println();
	
	
}

void Start()
{	
	int test = 0;
	while (1)
	{
		if (ReceiveString[test] == '{')
		{			
			Serial.println("Start");
			Serial.print("第");
			Serial.print(test+1);
			Serial.println("個 ");
			test++;
			if (ReceiveString[test] == '"')
			{
				Serial.print("第");
				Serial.print(test+1);
				Serial.println("個 ");
				break;
			}
		}
		test++;
	}	
	
}
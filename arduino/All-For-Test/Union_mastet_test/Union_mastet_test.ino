#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "math.h"
#include <Wire.h>
#include <arduino.h>

#define slave4 0x04
#define MySerial Serial
#define myserial MySerial
#define p(aa) MySerial.print(aa);
#define pl(aa) MySerial.println(aa);
#define debug 1

char *choose;
int suck;
int32_t suck_turn;
int jsonNumber = 0;
char json[200];

union unionType
{
	byte a[8] = { 0,0,0,0,0,0,0,0 };
	int32_t b;
};

void setup() 
{
	Wire.begin();
	MySerial.begin(115200);
	MySerial.println("**  Start **");
	choose = "!";
}

void loop()
{
	ForJson();
	if (strcmp(choose, "3") == 0)
	{		
		turn();
	}
}

void turn()
{
	unionType Slave4;
	//Slave4.b = (suck_turn / 0.1125);
	Slave4.b = (suck_turn);
	Wire.beginTransmission(slave4);
	Wire.write("turn");
	Wire.endTransmission();
	Wire.beginTransmission(slave4);
	for (int i = 0; i < 4; i++)
	{
		p("i = ");
		myserial.println(i);
		if (Slave4.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave4.a[i]);
		MySerial.println(Slave4.a[i]);
	}
	Wire.endTransmission();
}

void ForJson()	//*{"choose":"3","suck":1,"suck_turn":2}
{
	int a = 0;
	jsonNumber = 0;	
	choose = "";	
	suck = 0;
	suck_turn = 0;
	while (1)
	 {
		if (MySerial.available())
		{
			json[jsonNumber] = MySerial.read();
			if (json[jsonNumber] == '{')
			{
				a++;
			}
			else;
			if (json[jsonNumber] == '}')
			{
				a++;
			}
			else;
			jsonNumber++;
		}
		else;
		if (a == 2)
		{
			MySerial.println(json);
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			choose = root["choose"];			
			suck = root["suck"];
			suck_turn = root["suck_turn"];
			if (debug)
			{
				p("*choose = ");
				MySerial.println(choose);	
				p("*suck = ");
				MySerial.println(suck);
				p("*suck_turn = ");
				MySerial.println(suck_turn);
				MySerial.println(suck_turn,BIN);
			}
			a = 0;
			break;
		}
	 }
}
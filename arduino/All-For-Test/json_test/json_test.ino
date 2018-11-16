#include <ArduinoJson.hpp>
#include <ArduinoJson.h>

/*
{"choose":"1","X":11,"Y":6,"Z":2,"suck":1}
{"choose":"0","X":11,"Y":6,"Z":2,"suck":1}
*/
//char json[] = "{\"sensor\":\"gps\",\"time\":1351824120}";
//String json = "{\"sensor\":\"gps\",\"time\":1351824120}";
char json[200];
int jsonNumber = 0;

char *choose = "!";  //選擇輸入模式
double P[3] = { 0, 0, 0 };  //本次輸入的位置

void setup() 
{ 
	Serial.begin(9600);	
} 

void loop() 
{	
	int a = 0;
	while (1)
	{
		if (Serial.available())
		{
			json[jsonNumber] = Serial.read();			
			if (json[jsonNumber] == '{')
			{
				a++;
			}

			if (json[jsonNumber] == '}')
			{
				a++;
			}
			jsonNumber++;
		}

		if (a == 2)
		{
			Serial.println(json);		
			StaticJsonBuffer<400> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			choose = root["choose"];
			P[0] = root["X"];
			P[1] = root["Y"];
			P[2] = root["Z"];
			Serial.println(choose);	
			Serial.println(P[0]);
			Serial.println(P[1]);
			Serial.println(P[2]);

			a = 0;		
			delay(10);
		}
	}

}


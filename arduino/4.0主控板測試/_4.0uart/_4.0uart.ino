#include <Mifes.h>
#include <ArduinoJson.h>

String str;
int a;

void setup()
{
	Serial.begin(115200);
}

void loop() 
{	
	Show();
	delay(2000);	
}

void Show()
{
	Serial.println("{\"choose\":\"0\"}");
	delay(2000);
	Serial.println("{\"choose\":\"1\",\"X\": -16.5,\"Y\": 7.5,\"Z\": 5,\"suck\": 0 }");
	delay(2000);
	Serial.println("{ \"choose\":\"1\",\"X\" : 1,\"Y\" : 19,\"Z\" : 2,\"suck\" : 1 }");
	delay(2000);
	
	a = 0;
}




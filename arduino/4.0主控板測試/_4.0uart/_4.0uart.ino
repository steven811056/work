#include <Mifes.h>
#include <ArduinoJson.h>

String str;
int a;

void setup()
{
	Serial.begin(9600);
}

void loop() 
{	
	Show();
	delay(2000);	
}

void Show()
{
	Serial.println("{\"choose\":\"0\",\"X\":11,\"Y\":6,\"Z\":2,\"suck\":1}");
	a = 0;
}




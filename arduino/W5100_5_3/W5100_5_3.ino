#include <EEPROM.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,50,200 };
byte server[] = { 192,168,50,156 }; // Google

EthernetClient client;
StaticJsonBuffer<600> jsonBuffer;

char test;
char test2;
char jsonC[]="";
String jsonG;
String json2 = "{\"name\":\"steven\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char jsonD[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
int a = 0;
int k = 0;
int b = 0;

void setup()
{
	Serial.print("test----");
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json2);
	const char* name = root["name"];
	Serial.println(name);
	Serial.println("test----end");

	Ethernet.begin(mac, ip);
	Serial.begin(9600);
	delay(1000);
	Serial.println("connecting...");

	if (client.connect(server, 100)) {
		Serial.println("connected");
		client.println("GET ");
		client.println();
	}
	else {
		Serial.println("connection failed");
	}
	//Serial.print("EEPROM =");
	//Serial.println((char)EEPROM.read(1));
	//
	//if (EEPROM.read(0)!= 10)
	//{

	//}
	
	
}

void loop()
{	
	Serial.println(b);
	if (client.available())
	{
		test = client.read();
		Serial.println(test);
		Serial.println(k);
		jsonG = jsonG+test;		
		Serial.println(jsonG);		
		delay(1);
		if (jsonG[k] == '{')
		{
			a++;
			Serial.print("a = ");
			Serial.println(a);
		}
		
		if (jsonG[k] == '}')
		{
			a++;
			Serial.print("a = ");
			Serial.println(a);
		}
		k++;
		
	}
	
	
	if (a == 2)
	{
		int l = 10;
		while (jsonG[b] != '}')
		{
			b++;
		}
		Serial.println(b+1);
		for (int i = 0; i <b; i++)
		{
			/*jsonC[i] = jsonG[i];
			Serial.print(jsonC[i]);		*/	
		}
		Serial.println();
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsonG);
		const char* name = root["name"];
		Serial.println(name);		
		delay(10);
		client.write("{\"name\":\"");
		client.write(name);
		client.write("\"}");
		a = 0;
		jsonG = "";
		k = 0;
		b = 0;
		delay(10);
	}
	
}


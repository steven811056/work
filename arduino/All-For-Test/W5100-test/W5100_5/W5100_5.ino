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



char json[] = "{\"name\":\"steven\"}";
char jsonC[] = "1253";
char test;
String test2 = "";
String json2 = "{\"name\":\"steven\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char jsonD[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
int a = 0;
int i2 = 0;
int k = 0;

void setup()
{
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

const char* name ;
long time ;
double latitude;
double longitude;




void testjson(JsonObject& root)
{
	strcpy(jsonC, json);
	const char* name = root["name"];
	long time = root["time"];
	double latitude = root["data"][0];
	double longitude = root["data"][1];
}
void scan()
{
	testjson(jsonBuffer.parseObject(jsonC));
}
void loop()
{		
	//Serial.println(json);
	if (client.available()) {
		Serial.print("client.available =");
		Serial.println(client.available());
		Serial.println();
		delay(1);			
		for (int i = 0; i < client.available(); i++)
		{
			 json[k] = client.read();
			 if (json[k] == '{')
			 {
				 a++;
			 }
			 if (json[k] == '}')
			 {
				 a++;
			 }				
			 Serial.println(k);
			 Serial.print("json=");
			 Serial.println(json[k]);
			 Serial.println();
			 delay(10);			 
			 k++;
		}
		delay(1);		
		
		Serial.println();
		Serial.print("a=");
		Serial.println(a);
		
		
	}		
	if (a == 2)
	{
		
		Serial.print("sizeof json=");
		Serial.println(sizeof(json));
		Serial.println();
		for (int i = 0; i < 1; i++)
		{
			Serial.print("OK  a=2------  ");			
			Serial.println(jsonC);				
			Serial.print("name----- ");
			Serial.println(name);
			Serial.print("time------ ");
			Serial.println(time);

			a = 0;
		}
		
	}

	/*if (!client.connected()) {
		Serial.println();
		Serial.println("disconnecting.");
		client.stop();
		Serial.println("123456654654566");
		for (;;);
	}*/
}


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
char jsonC[200] = "";
String jsonG;

void setup()
{		
	Ethernet.begin(mac, ip);
	Serial.begin(9600);	
	Serial.println("connecting...");
	if (client.connect(server, 100)) {
		Serial.println("connected");
		client.println("GET ");
		client.println();
	}
	else {
		Serial.println("connection failed");
	}
	
}

void loop()
{
	int a = 0;	
	int k2 = 0;	
	while (1)
	{		
		if (client.available())
		{
			test = client.read();    
			jsonG = jsonG + test;						
			if (jsonG[k2] == '{')
			{
				a++;
			}

			if (jsonG[k2] == '}')
			{
				a++;			
			}
			k2++;			
		}		

		if (a == 2)
		{	
			Serial.println(jsonG);
			JsonObject& root = jsonBuffer.parseObject(jsonG);
			const char* name = root["name"];
			Serial.println(name);
			delay(10);
			client.write("name = ");
			client.write(name);			
			delay(5);
			client.write(jsonG.c_str());			
			client.write("--OK--");
			a = 0;
			jsonG = "";			
			k2 = 0;			
			delay(10);
		}
	}
}


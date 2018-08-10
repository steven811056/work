#include <EEPROM.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

EthernetClient client;
StaticJsonBuffer<600> jsonBuffer;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,50,200 };
byte server[] = { 192,168,50,156 }; // Google
String json2 = "{\"name\":\"steven\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char test;
char test2;
String jsonG;
char jsonD[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

int k = 0;
int b = 0;
char h = '1';
char jsonC[] = "";
int jsonT[] = {0};
char jsonC2[10] = { '1','2', 'k' };
void setup() 
{
	Serial.begin(9600);
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
}

void loop() 
{
	int a = 0;
	while (1)
	{
		if (client.available()>0)
		{
			test = client.read();
			b = test;
			Serial.println(test);
			Serial.println(k);
			k++;
			delay(10);
		}
		if (k > 0 && a<20)
		{
			Serial.print("a1 = ");
			Serial.println(a);
			jsonC[a] = test;
			Serial.print("a2 = ");
			Serial.println(a);
			Serial.println(jsonC[a]);
			a++;
			Serial.print("a3 = ");
			Serial.println(a);
			k=0;
		}
		
		/*json2[5] = 'n';
		jsonC[0] = 'h';
		Serial.println(jsonC[0]);*/
		/*for (int i = 0; i < 9; i++)
		{
			Serial.println(i);

			jsonC2[i] = h;
		}
		for (int i = 0; i < 3; i++)
		{
			Serial.println("jsonC2 = ");
			Serial.println(jsonC2[i]);
		}*/
	}
	return;
	
}

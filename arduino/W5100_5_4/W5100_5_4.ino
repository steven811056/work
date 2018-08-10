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

//char* dstr;

char test;
char test2;
char jsonC[200] = "";
String jsonG;
String json2 = "{\"name\":\"steven\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
char jsonD[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

void setup()
{
	/*
	dstr = new char[10];
	char* dstr2 = new char[11];

	//for ...
	delete[] dstr;
	dstr = dstr2;
	*/

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
  int a = 0;
  int k = 0;
  int b = 0;
  int k2 = 0;
  int q = 0;
  while(1)
  {
	if (client.available())
	{
		test = client.read();
	//      Serial.println(test);
	//      Serial.println(k);     
		jsonG = jsonG + test;
		Serial.println(jsonG);
		delay(5);
		if (jsonG[k2] == '{')
		{
		a++;
		Serial.print("a = ");
		Serial.println(a);
		}
  
		if (jsonG[k2] == '}')
		{
		a++;
		Serial.print("a = ");
		Serial.println(a);
		}
		k2++;
		k++;
		

	}
    if(k>0)
    {
		jsonC[b]=test;
		Serial.print("jsonC = ");
		Serial.println(jsonC[b]);
		Serial.print("b = ");
		Serial.println(b);
		b++;
		k=0;
    }
    
    if (a == 2)
    {		
		//jsonC[1] = jsonC[1] - 91;
		Serial.println(jsonC);
		if (jsonC[18] != '\0')
		{
			q++;
		}
		delay(10);	//?????
		while (jsonC[q] != '\0')
		{
			q++;
		}
		delay(10);
		Serial.println(q);
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(jsonG);
		const char* name = root["name"];
		Serial.println(name);
		delay(10);
		//client.write("{\"name\":\"");
		client.write(jsonC);
		//client.write(jsonG.c_str());
		//client.write("\"}");
		client.write("--OK--");
		a = 0;
		jsonG = "";		
		k = 0;
		k2 = 0;
		b = 0;
		delay(10);
    }
  }
}


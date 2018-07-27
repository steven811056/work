#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,50,200 };
byte server[] = { 192,168,50,156 }; // Google

EthernetClient client;

void setup()
{
	Ethernet.begin(mac, ip);
	Serial.begin(9600);

	delay(1000);

	Serial.println("connecting...");

	if (client.connect(server, 100)) {
		Serial.println("connected");
		client.println("GET /search?q=arduino HTTP/1.0");
		client.println();
	}
	else {
		Serial.println("connection failed");
	}
}

void loop()
{
	if (client.available()) {
		char c = client.read();
		Serial.print(c);				
	}

	if (!client.connected()) {
		Serial.println();
		Serial.println("disconnecting.");
		client.stop();
		Serial.println("123456654654566");
		for (;;);
	}
}
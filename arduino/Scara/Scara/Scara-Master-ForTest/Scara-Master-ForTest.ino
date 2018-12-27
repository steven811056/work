#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <math.h>
#include <Wire.h>
#include <arduino.h>
#include <for_String.h>

#define MySerial Serial
#define debug 1

int8_t Max_Speed, Add_Speed;	//最大速度與加速度
int jsonNumber = 0;
double P[3] = { 0, 0, 0 };  //本次輸入的位置
char* choose;	//選擇輸入模式
int suck;		//吸盤

void setup()
{
	Wire.begin();
	MySerial.begin(115200);

}

string_turn for_json_string_;

void loop() 
{
	MySerial.println("Start-forjson");
	ForJson();
	MySerial.println("End-forjson");
}

void ForJson()
{	
forjson_1:
	for_json_string_.String_reset();
	while(for_json_string_.String_end() != 1)
	{		
		for_json_string_.String_start();		
	}
	/*MySerial.println("forjson-buffer");
	MySerial.println(for_json_string_.json_string);*/
	StaticJsonBuffer<200> jsonBuffer;	
	JsonObject& root = jsonBuffer.parseObject(for_json_string_.json_string);		
	if (!root.success())
	{
		MySerial.println("forjson-not-success");
		for_json_string_.String_reset();
		goto forjson_1;
	}		
	choose = root["choose"];
	P[0] = root["X"];
	P[1] = root["Y"];
	P[2] = root["Z"];
	suck = root["suck"];
	Max_Speed = root["pps"];
	Add_Speed = root["acc"];
	if (debug)
	{		
		MySerial.println(choose);
		MySerial.println(P[0]);
		MySerial.println(P[1]);
		MySerial.println(P[2]);
		MySerial.println(suck);
		MySerial.println(Max_Speed);
		MySerial.println(Add_Speed);
	}
	while (MySerial.available() > 0)
	{
		MySerial.read();
	}
}


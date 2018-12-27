#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "math.h"
#include <Wire.h>
#include <for_String.h>
#include <arduino.h>

#define slave1 0x01
#define slave4 0x04
#define MySerial Serial
#define myserial MySerial
#define p(aa) MySerial.print(aa);
#define pl(aa) MySerial.println(aa);
#define debug 1
#define microstep 0.1125		//�L�B����v	16

int32_t turn_json;
double P[3] = { 0, 0, 0 };  //������J����m
			    //-----
char *choose;
//String choose;
int suck;
//	>>>	�[�t��
int16_t Max_Speed, Add_Speed;	//�̤j�t�׻P�[�t��
double Max_step, add_step;	//�ഫ���B��
int32_t parabola_constant;	//�ߪ��u��{��	�`��
int32_t parabola_x;	//�ߪ��u��{��	X
double add_delaytime;	//�[�t��ؼгt�׻ݭn���ɶ�
int16_t Max_distance;	//���ʶZ��
double all_delaytime;		//��ؼжZ���ݭn���ɶ�

int32_t suck_turn;
int jsonNumber = 0;
char json[200];

union unionType
{
	byte a[8] = { 0,0,0,0,0,0,0,0 };
	int32_t b;
};

void setup() 
{
	Wire.begin();
	MySerial.begin(115200);
	MySerial.println("**  Start **");
	choose = "!";
}

string_turn for_json_string_;

void loop()
{
	ForJson();
	turn();	
}

void turn()
{
	unionType Slave1;
	//Slave4.b = (suck_turn / 0.1125);
	Slave1.b =Add_Speed;	
	Wire.beginTransmission(slave1);
	for (int i = 0; i < 4; i++)
	{		
		Wire.write(Slave1.a[i]);
		MySerial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
}

void json_reset()
{
	choose = "";
	turn_json = 0;
}


void ForJson()
{
	json_reset();
forjson_1:
	for_json_string_.String_reset();
	while (for_json_string_.String_end() != 1)
	{
		for_json_string_.String_start();
	}
	MySerial.println("forjson-buffer");
	MySerial.println(for_json_string_.json_string);
	StaticJsonBuffer<150> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(for_json_string_.json_string);
	if (!root.success())
	{
		MySerial.println("forjson-not-success");
		for_json_string_.String_reset();
		goto forjson_1;
	}
	//	>>>	��char*�}�C�b�Y��MCU�sĶ�ɷ|�����D
	String json_string = root["choose"];
	for (int i = 0; i<sizeof(json_string); i++)
	{
		choose[i] = json_string[i];
	}
	P[0] = root["p"][0];
	P[1] = root["p"][1];
	P[2] = root["p"][2];
	turn_json = root["turn"];
	turn_json = (turn_json * 360) / microstep;
	Max_Speed = root["pps"];
	Add_Speed = root["acc"];
	MySerial.println(choose);
	MySerial.println(turn_json);
	MySerial.println(Max_Speed);
	MySerial.println(Add_Speed);
	while (MySerial.available() > 0)
	{
		MySerial.read();
	}
}
#include <arduino.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "DeltaRobInverseKin.h"
#include "math.h"
#include "My.h"
#include <for_String.h>
#include <Wire.h>

#define L_UPPER 0.07 //上臂長		單位：公尺
#define L_LOWER 0.285 //下臂長
#define WB 0.13  //底座正三角型重心到邊的長度
#define WP 0.013  //下方小正三角形重心到邊長的長度
#define UP 0.026  //下方小正三角形重心到頂點的長度
#define SP 0.045  //下方正三角型的邊長
#define microstep 0.1125		//微步控制倍率	16
//#define microstep 0.225 	//微步控制倍率	8

#define MySerial Serial
#define My_Serial Serial
#define MySlave1 0x01
#define MySlave2 0x02
#define MySlave3 0x03
#define MySlave4 0x04
boolean debug = 1;

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
//--json
int32_t turn_json;
double P[3] = { 0, 0, 0 };  //本次輸入的位置
//-----
char *choose;
int suck;
//	>>>	加速度
int16_t Max_Speed, Add_Speed;	//最大速度與加速度
double Max_step, add_step;	//轉換成步數
int32_t parabola_constant;	//拋物線方程式	常數
int32_t parabola_x;	//拋物線方程式	X
double add_delaytime;	//加速到目標速度需要的時間
int16_t Max_distance;	//移動距離
double all_delaytime;		//到目標距離需要的時間
//	<<<	加速度
double coordinate[3];			//輸入的x , y , z
int16_t D[3] = { 0,0,0 };	//抓取演算法出來的goalPos的角度
uint8_t returnData = 0;	//歸零完傳回的數值
double degreeTurn[1][3];		//與前一次相減後運轉方向與角度
double location[2][3];	//用來存之前的位置
int16_t ring;		//輸入位置的次數  暫時沒用到
byte contral_number;		//控制指令

union unionType
{
	byte a[4];
	int32_t b;	//存轉動的值	
};

void setup()
{
	My_Serial.begin(115200);
	Wire.begin();
	delta.debugFlag = false;
	My_Serial.println(" start ");
	My_Serial.println();
	for (int i = 0; i < 2; i++)
	{
		for (int i2 = 0; i2< 3; i2++)
		{
			location[i][i2] = 0;
		}
	}
	for(int i=20;i<=23;i++)
	{
		pinMode(i, OUTPUT);
		digitalWrite(i,HIGH);
	}	
}

string_turn for_json_string_;

void loop()
{
	ForJson();
	//Show();	
	My_Serial.println();
	My_Serial.println("輸入目標點的X , Y , Z");
	PtoP_input();
	ctrl_PtoP();	
}

//----PtoP-----輸入X Y Z的函式
void PtoP_input()
{
	My_Serial.println("PtoP input		Start ");
	int D = 0;
	for(int i=0;i<3;i++)
	{
		coordinate[i] = P[i];
	}
	if(debug)
	{
		My_Serial.print("目前X , Y , Z 軸參數 = ");
		My_Serial.print(coordinate[0]);
		My_Serial.print(" , ");
		My_Serial.print(coordinate[1]);
		My_Serial.print(" , ");
		My_Serial.print(coordinate[2]);
		My_Serial.println();		
		My_Serial.println("PtoP input		END ");
	}		
}
//----PtoP-----end----------

//-------ctrl_PtoP---------
void ctrl_PtoP()		//輸入x,y,z進來
{
	My_Serial.print("ctrl_PtoP  ");
	My_Serial.println("Start");
	//輸入 0 0 0 進入歸零狀態
	if (coordinate[0] == 0 && coordinate[1] == 0 && coordinate[2] == 0)
	{
		//DeltaTest.return1();		
		reset_delta();
		AfterCompare();		//對前一次的位置進行覆寫
		goto end;
	}
	My_Serial.print(coordinate[0]);
	My_Serial.print(" , ");
	My_Serial.print(coordinate[1]);
	My_Serial.print(" , ");
	My_Serial.print(coordinate[2]);
	My_Serial.println();
	delta.setGoalCoordinates(coordinate[0] *0.01, coordinate[1] *0.01, coordinate[2] *0.01, 0);	
	//進入位置比對
	LocationCompare();
	//PtoP_output();
	if (coordinate[0] != 0 || coordinate[1] != 0 || coordinate[2] != 0)
	{
		turn();
	}
end:
	My_Serial.print("ctrl_PtoP			END		 ");
}
//-------ctrl_PtoP---------end--------

//-------- LocationCompare -------------
void LocationCompare()		//現在位置與前一次的比對
{
	My_Serial.println("LocationCompare		start	");
	for (int i = 0; i < 3; i++)
	{
		location[0][i] = delta.goalPos[i];
	}
	for (int i = 0; i < 3; i++)
	{
		My_Serial.print("location[0]");
		My_Serial.println(location[0][i]);
		My_Serial.print("location[1]");
		My_Serial.println(location[1][i]);
		My_Serial.println();
	}
	if (location[0] != location[1])
	{
		My_Serial.println("location[0] != location[1]");
		for (int i = 0; i < 3; i++)
		{
			degreeTurn[0][i] = location[0][i] - location[1][i];
			My_Serial.print("degreeTurn = ");
			My_Serial.println(degreeTurn[0][i]);
		}
		AfterCompare();
	}
}
//-------- LocationCompare -------------end ------------

//------turn-----
void turn()		//馬達轉動函式
{	
	//reset_delta();	
	turn_velocity();
	turn_acc();	
	turn_degree();	
	turn_start();
}
//------turn-----end--------
//	>>>	turn velocity acc
void turn_velocity()	//contral_number	3
{
	unionType Slave1;	
	contral_number = 3;
	Slave1.b = Max_Speed;
	Wire.beginTransmission(MySlave1);
	Wire.write(contral_number);	
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave1.a[i]);
		Serial.println(Slave1.a[i]);
	}
	Wire.endTransmission();	
	Serial.println("turn_velocity	end");
}
//	<<<	turn velocity acc
void turn_acc()	//	contral_number	4
{
	unionType Slave1;
	Slave1.b = Add_Speed;	
	contral_number = 4;
	Wire.beginTransmission(MySlave1);
	Wire.write(contral_number);	
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave1.a[i]);
		Serial.println(Slave1.a[i]);
	}
	Wire.endTransmission();	
	Serial.println("turn_acc	end");
}
//	>>>	turn degree
void turn_degree()	//	contral_number		2	角度
{
	contral_number = 2;
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Slave1.b = degreeTurn[0][0] / microstep;
	Slave2.b = degreeTurn[0][1] / microstep;
	Slave3.b = degreeTurn[0][2] / microstep;	
	if (debug)
	{
		MySerial.println("角度 1 2 3 ");
		MySerial.println(degreeTurn[0][0]);
		MySerial.println(Slave1.b);
		MySerial.println(degreeTurn[0][1]);
		MySerial.println(Slave2.b);
		MySerial.println(degreeTurn[0][2]);
		MySerial.println(Slave3.b);
	}
	Wire.beginTransmission(MySlave1);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave1.a[i]);		
	}
	Wire.endTransmission();	
	Wire.beginTransmission(MySlave2);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave2.a[i]);		
	}
	Wire.endTransmission();
	Wire.beginTransmission(MySlave3);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave3.a[i]);		
	}
	Wire.endTransmission();	
}
//	<<<	turn degree	角度
//	>>>	turn start
void turn_start()	//	>>>	方向	contral_number 1
{
	unionType Slave;
	contral_number = 1;
	Slave.b = 2;
	Wire.beginTransmission(MySlave1);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave.a[i]);		
	}
	Wire.endTransmission();
	//
	Wire.beginTransmission(MySlave2);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave.a[i]);		
	}
	Wire.endTransmission();
	//
	Wire.beginTransmission(MySlave3);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave.a[i]);		
	}
	Wire.endTransmission();
}	//	<<<	方向
	//	<<<	turn start

//---------AfterCompare-----------
void AfterCompare()		//對前一次的位置進行覆寫
{
	for (int i = 0; i < 3; i++)
	{
		location[1][i] = location[0][i];
	}
}
//---------AfterCompare-----------end-----

//--------PtoP_output----------
void PtoP_output()
{
	My_Serial.println();
	My_Serial.println("PtoP_output");
	LocationCompare();

	My_Serial.print("三顆馬達轉動角度 = ");
	for (int i = 0; i < 3; i++)
	{
		My_Serial.print(delta._theta2[i]);
		if (i < 2)
		{
			My_Serial.print(" , ");
		}
	}
	My_Serial.println();
	My_Serial.print("三顆馬達轉動角度 2= ");
	for (int i = 0; i < 3; i++)
	{
		My_Serial.print(D[i]);
		if (i < 2)
		{
			My_Serial.print(" , ");
		}
	}
	if (delta.maxArrIndex > 1)
	{
		My_Serial.println();
		My_Serial.println(delta.maxArrIndex);
		for (int i = 0; i < 3; i++)
		{
			My_Serial.println(location[delta.maxArrIndex - 2][i]);
		};
		My_Serial.print("兩次輸入度數差 = ");
		for (int i = 0; i < 3; i++)
		{
			My_Serial.print(degreeTurn[0][i]);
			if (i < 2)
			{
				My_Serial.print(" , ");
			}
		}
	}

}
//--------PtoP_output----------end-----

//	>>>	緊急停止相關
void E_Stop()
{
	contral_number = 6;
	if (strcmp(choose, "stop") == 0)
	{
		MySerial.println("緊急停止");
		Wire.beginTransmission(MySlave1);
		Wire.write(contral_number);
		Wire.endTransmission();
		Wire.beginTransmission(MySlave2);
		Wire.write(contral_number);
		Wire.endTransmission();
		Wire.beginTransmission(MySlave3);
		Wire.write(contral_number);
		Wire.endTransmission();
	}
}

void E_Start()
{
	contral_number = 7;
	if (strcmp(choose, "stop") == 0)
	{
		MySerial.println("緊急停止結束");
		Wire.beginTransmission(MySlave1);
		Wire.write(contral_number);
		Wire.endTransmission();
		Wire.beginTransmission(MySlave2);
		Wire.write(contral_number);
		Wire.endTransmission();
		Wire.beginTransmission(MySlave3);
		Wire.write(contral_number);
		Wire.endTransmission();
	}
}
//	<<<	

//---------reset_delta---------
void reset_delta()	//歸零函式
{	
	contral_number = 0;
	Wire.beginTransmission(MySlave1);
	Wire.write(contral_number);	
	Wire.endTransmission();
	Wire.beginTransmission(MySlave2);	
	Wire.write(contral_number);
	Wire.endTransmission();
	Wire.beginTransmission(MySlave3);
	Wire.write(contral_number);	
	Wire.endTransmission();	
}
//---------reset_delta---------end----------

void suck_plate()
{
	if (debug)
	{
		//Serial.println("beginTransmission(4)");	
		Serial.println("beginTransmission(3)");
	}
	else;
	if (suck == 0)
	{
		//Wire.beginTransmission(MySlave4);
		Serial.println("suck 0");
		Wire.beginTransmission(MySlave3);
		Wire.write("close");
		Wire.endTransmission();
	}
	else if (suck == 1)
	{
		Serial.println("suck 1");
		Wire.beginTransmission(MySlave3);
		Wire.write("open");
		Wire.endTransmission();
	}
	else;
}

//	>>>	json相關
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
	//	>>>	用char*陣列在某些MCU編譯時會有問題
	String json_string = root["func"];
	for(int i=0;i<sizeof(json_string);i++)
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
//	<<<	Forjson
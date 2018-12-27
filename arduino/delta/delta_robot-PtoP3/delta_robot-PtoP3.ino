#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include "DeltaRobInverseKin.h"
#include "math.h"
#include "My.h"
#include <for_String.h>
#include <Wire.h>

#define L_UPPER 0.12 //上臂長		單位：公尺
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
char json[200];
int jsonNumber = 0;
int32_t turn_json;
double P[3] = { 0, 0, 0 };  //本次輸入的位置
//-----
char *choose;
int suck;

int16_t Max_Speed, Add_Speed;	//最大速度與加速度
double Max_step, add_step;	//轉換成步數

double coordinate[3];			//輸入的x , y , z
int16_t D[3] = { 0,0,0 };	//抓取演算法出來的goalPos的角度
uint8_t returnData = 0;	//歸零完傳回的數值
double degreeTurn[1][3];		//與前一次相減後運轉方向與角度
double location[2][3];	//用來存之前的位置
int16_t locationNumber = 0;
int16_t ring;		//輸入位置的次數  暫時沒用到
int shownumber;


union unionType
{
	byte a[4];
	int32_t b;
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

}

string_turn for_json_string_;

void loop()
{		
	ForJson();
	
	Show();
	/*while (1)
	{
	My_Serial.println();
	My_Serial.println("輸入目標點的X , Y , Z");
	PtoP_input();
	ctrl_PtoP(coordinate[0], coordinate[1], coordinate[2]);
	}*/
}

//----PtoP-----輸入X Y Z的函式
void PtoP_input()
{
	My_Serial.println("PtoP input		Start ");
	int D = 0;
	while (1)
	{
		if (D == 0)
		{
			My_Serial.println("輸入X軸參數");
			while (1)
			{
				if (My_Serial.available())
				{
					coordinate[0] = My_Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 1)
		{
			My_Serial.println("輸入Y軸參數");
			while (1)
			{
				if (My_Serial.available())
				{
					coordinate[1] = My_Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 2)
		{
			My_Serial.println("輸入Z軸參數");
			while (1)
			{
				if (My_Serial.available())
				{
					coordinate[2] = 0 - My_Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 3)
		{
			My_Serial.print("目前X , Y , Z 軸參數 = ");
			My_Serial.print(coordinate[0]);
			My_Serial.print(" , ");
			My_Serial.print(coordinate[1]);
			My_Serial.print(" , ");
			My_Serial.print(coordinate[2]);
			My_Serial.println();
			D = 0;
			My_Serial.println("PtoP input		END ");
			break;
		}

	}
}
//----PtoP-----end----------

//-------ctrl_PtoP---------
void ctrl_PtoP(int16_t x0, int16_t y0, int16_t z0)	//輸入x,y,z進來
{
	My_Serial.print("ctrl_PtoP  ");
	My_Serial.println("Start");
	//輸入 0 0 0 進入歸零狀態
	if (coordinate[0] == 0 && coordinate[1] == 0 && coordinate[2] == 0)
	{
		//DeltaTest.return1();		
		reset_delta();
		AfterCompare();		//對前一次的位置進行覆寫
	}
	My_Serial.print(x0);
	My_Serial.print(" , ");
	My_Serial.print(y0);
	My_Serial.print(" , ");
	My_Serial.print(z0);
	My_Serial.println();
	delta.setGoalCoordinates(x0*0.01, y0*0.01, z0*0.01, 0);
	delay(10);
	//進入位置比對
	LocationCompare();
	//PtoP_output();
	if (coordinate[0] != 0 || coordinate[1] != 0 || coordinate[2] != 0)
	{
		turn();

	}
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
	Wire.beginTransmission(1);
	if (debug)
	{
		My_Serial.println("-->Turn -->beginTransmission to 1");
	}
	Wire.write("start");
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println((int)degreeTurn[0][0]);
	}
	Wire.beginTransmission(1);	
	Wire.write((int)degreeTurn[0][0]);
	Wire.endTransmission();
	//-----2-----
	Wire.beginTransmission(2);
	if (debug)
	{
		My_Serial.println("-->Turn -->beginTransmission to 2");
	}
	Wire.write("start");
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println((int)degreeTurn[0][1]);
	}
	Wire.beginTransmission(2);	
	Wire.write((int)degreeTurn[0][1]);
	Wire.endTransmission();
	//-----3-----.	
	Wire.beginTransmission(3);
	if (debug)
	{
		My_Serial.println("-->Turn -->beginTransmission to 3");
	}
	Wire.write("start");
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println((int)degreeTurn[0][2]);
	}
	Wire.beginTransmission(3);	
	Wire.write((int)degreeTurn[0][2]);
	Wire.endTransmission();

}
//------turn-----end--------

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

//---------reset_delta---------
void reset_delta()	//歸零函式
{	
	Wire.beginTransmission(0x01);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(0x02);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(0x03);
	Wire.write("reset");
	Wire.endTransmission();
}
//---------reset_delta---------end----------

void Show()
{
	if (debug)
	{
		My_Serial.println("Show");
	}
	else;
	if (strcmp(choose, "0") == 0)
	{
		Serial.println("歸零");
		reset_delta();
	}
	else if (strcmp(choose,"1") == 0)
	{
		Serial.println("轉動 (短)");
		Show_Turn_All(0,25,153);
	}
	else if (strcmp(choose, "2") == 0)
	{
		Serial.println("轉動 (長)");
		Show_Turn_All(7, 22, 7);
	}	
	else if (strcmp(choose, "4") == 0)
	{
		Serial.println("轉動 (長)");
		Show_Turn_All(135, 0, 0);
	}
	else if (strcmp(choose, "5") == 0)
	{
		Serial.println("轉動 (長)");
		Show_Turn_All(0, 10, 10);
	}
	else if (strcmp(choose, "3") == 0)
	{
		Serial.println("吸盤");
		suck_plate();
	}	
}

void Show_Turn_All(int a ,int b, int c)
{
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Slave1.b = stepOpen(a);
	Slave2.b = stepOpen(b);
	Slave3.b = stepOpen(c);
	if (debug)
	{
		My_Serial.println(Slave1.b);
		My_Serial.println(Slave2.b);
		My_Serial.println(Slave3.b);
	}
	Wire.beginTransmission(MySlave1);
	Wire.write("start2");
	Wire.endTransmission();
	Wire.beginTransmission(MySlave2);
	Wire.write("start2");
	Wire.endTransmission();
	Wire.beginTransmission(MySlave3);
	Wire.write("start2");
	Wire.endTransmission();
	delay(20);
	if (debug)
	{
		My_Serial.println("beginTransmission(1);");
	}	
	Wire.beginTransmission(MySlave1);
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave1.a[i]);
		Serial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println();
	}
	if (debug)
	{
		My_Serial.println("beginTransmission(2);");
	}
	Wire.beginTransmission(MySlave2);
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave2.a[i]);
		Serial.println(Slave2.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println();
	}
	if (debug)
	{
		My_Serial.println("beginTransmission(3);");
	}
	Wire.beginTransmission(MySlave3);
	for (int i = 0; i < 4; i++)
	{
		if (Slave3.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave3.a[i]);
		Serial.println(Slave3.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println();
	}
}

void Show_Turn()
{
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Slave1.b = stepOpen(0);
	Slave2.b = stepOpen(30);
	Slave3.b = stepOpen(160);
	if (debug)
	{
		My_Serial.println(Slave1.b);
		My_Serial.println(Slave2.b);
		My_Serial.println(Slave3.b);
	}
	Wire.beginTransmission(MySlave3);
	Wire.write("start2");
	Wire.endTransmission();
	delay(2000);
	Wire.beginTransmission(MySlave2);
	Wire.write("start2");
	Wire.endTransmission();
	delay(300);
	Wire.beginTransmission(MySlave1);
	Wire.write("start2");
	Wire.endTransmission();	
	if (debug)
	{
		My_Serial.println("beginTransmission(1);");
	}
	delay(20);
	Wire.beginTransmission(MySlave1);
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave1.a[i]);
		Serial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println();
	}
	if (debug)
	{
		My_Serial.println("beginTransmission(2);");
	}
	Wire.beginTransmission(MySlave2);
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave2.a[i]);
		Serial.println(Slave2.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println();
	}
	if (debug)
	{
		My_Serial.println("beginTransmission(3);");
	}
	Wire.beginTransmission(MySlave3);
	for (int i = 0; i < 4; i++)
	{
		if (Slave3.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave3.a[i]);
		Serial.println(Slave3.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println();
	}
}

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

//--------json字串讀取---------
//void ForJson()
//{
//	jsonNumber = 0;
//	int a = 0;
//	choose = "";	
//	suck = 0;
//	while (1)
//	{
//		if (My_Serial.available())
//		{
//			json[jsonNumber] = My_Serial.read();
//			if (json[jsonNumber] == '{')
//			{
//				a++;
//			}
//
//			if (json[jsonNumber] == '}')
//			{
//				a++;
//			}
//			jsonNumber++;
//		}
//
//		if (a == 2)
//		{
//			My_Serial.println(json);
//			StaticJsonBuffer<400> jsonBuffer;
//			JsonObject& root = jsonBuffer.parseObject(json);
//			String json_string = root["choose"];
//			for (int i = 0; i<sizeof(json_string); i++)
//			{
//				choose[i] = json_string[i];
//			}
//			suck = root["suck"];
//			if (debug)
//			{
//				My_Serial.println(choose);				
//				My_Serial.println(suck);
//			}
//			a = 0;
//			break;
//		}
//	}
//}
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
	String json_string = root["choose"];
	for (int i = 0; i<sizeof(json_string); i++)
	{
		choose[i] = json_string[i];
	}
	P[0] = root["X"];
	P[1] = root["Y"];
	P[2] = root["Z"];
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
//--------********json字串讀取---------

//---微步
int32_t stepOpen(int x)
{
	x = (x / 0.1125);
	return x;
}
//--*****-微步-----
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
#define microstep 0.05625		//微步控制倍率	32
//#define microstep 0.1125		//微步控制倍率	16
//#define microstep 0.225 	//微步控制倍率	8

#define MySerial Serial
#define My_Serial Serial
#define MySlave1 0x01
#define MySlave2 0x02
#define MySlave3 0x03
#define MySlave4 0x04
#define debug_on 1
#define debug_off 0

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
string_turn for_json_string_(debug_on);
//--json
int32_t turn_json;
double P[3] = { 0, 0, 0 };  //本次輸入的位置	單位為公尺
//-----
char *choose =0;
//String choose;
int n;	//單軸移動時存放號碼地方
int suck;
//	>>>	加速度
int16_t Max_Speed ;	//最大速度與加速度
int32_t Add_Speed;
double Max_step, add_step;		//轉換成步數
int32_t parabola_constant;	//拋物線方程式	常數
int32_t parabola_x;	//拋物線方程式	X
double add_delaytime;	//加速到目標速度需要的時間
int16_t Max_distance;	//移動距離
double all_delaytime;		//到目標距離需要的時間
//	<<<	加速度
double coordinate[3];			//輸入的x , y , z
int16_t D[3] = { 0,0,0 };	//抓取演算法出來的goalPos的角度
uint8_t returnData = 0;	//歸零完傳回的數值
int Slave_direction[3];	//與前一次相減後運轉方向
double degreeTurn[3];		//與前一次相減後運轉角度
double location[2][3];	//用來存之前的位置
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
	My_Serial.println(" start  \n");		
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

void loop()
{
	boolean loop_debug = debug_on;
	ForJson();
	if (strcmp(choose,"movej") == 0)
	{		
		if (loop_debug)
		{
			My_Serial.println("** loop --> 輸入目標點的X , Y , Z");
		}		
		PtoP_input(debug_on);
		ctrl_PtoP(debug_on);
	}	
	if (strcmp(choose,"movesj") == 0)
	{		
		if (loop_debug)
		{
			My_Serial.println("** loop --> 單軸移動");
		}		
		movesj_func(debug_on);
	}
}

void movesj_func(boolean movesj_func_debug)
{
	if (turn_json > 0)
	{
		for (int i = 0; i < 3; i++)
		{
			Slave_direction[i] = 2;
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			Slave_direction[i] = 1;
		}
	}
	if (n == 1)
	{
		degreeTurn[0] = turn_json;
		degreeTurn[1] = 0;
		degreeTurn[2] = 0;		
	}
	if (n == 2)
	{
		degreeTurn[0] = 0;
		degreeTurn[1] = turn_json;
		degreeTurn[2] = 0;
		
	}
	if (n == 3)
	{
		degreeTurn[0] = 0;
		degreeTurn[1] = 0;
		degreeTurn[2] = turn_json;		
	}
	turn();
}

//----PtoP-----輸入X Y Z的函式
void PtoP_input(boolean PtoP_input_debug)
{
	if (PtoP_input_debug)
	{
		My_Serial.println("** PtoP input --> Start ");
	}	
	int D = 0;
	for(int i=0;i<3;i++)
	{
		coordinate[i] = P[i];
	}
	if(PtoP_input_debug)
	{
		My_Serial.print("目前X , Y , Z 軸參數 = ");
		My_Serial.print(coordinate[0]);
		My_Serial.print(" , ");
		My_Serial.print(coordinate[1]);
		My_Serial.print(" , ");
		My_Serial.print(coordinate[2]);
		My_Serial.println();		
		My_Serial.println("** PtoP input --> END \n");
	}		
}
//----PtoP-----end----------

//-------ctrl_PtoP---------
void ctrl_PtoP(boolean ctrl_PtoP_debug)		//輸入x,y,z進來
{
	if (ctrl_PtoP_debug)
	{
		My_Serial.print("** ctrl_PtoP  --> Start \n");
	}	
	//輸入 0 0 0 進入歸零狀態
	if (coordinate[0] == 0 && coordinate[1] == 0 && coordinate[2] == 0)
	{
		//DeltaTest.return1();		
		reset_delta();
		AfterCompare();		//對前一次的位置進行覆寫
		goto end;
	}	
	//	轉成公分
	delta.setGoalCoordinates(coordinate[0] *0.01, coordinate[1] *0.01, coordinate[2] *0.01, 0);
	//delta.setGoalCoordinates(coordinate[0], coordinate[1], coordinate[2] , 0);
	//進入位置比對
	LocationCompare(ctrl_PtoP_debug);
	//PtoP_output();
	if (coordinate[0] != 0 || coordinate[1] != 0 || coordinate[2] != 0)
	{
		turn();
	}
end:
	if (ctrl_PtoP_debug)
	{
		My_Serial.print("\n** ctrl_PtoP --> END \n\n ");
	}	
}
//-------ctrl_PtoP---------end--------

//-------- LocationCompare -------------
void LocationCompare(boolean LocationCompare_debug)		//現在位置與前一次的比對
{
	if (LocationCompare_debug)
	{
		My_Serial.println("** LocationCompare --> start");
	}	
	for (int i = 0; i < 3; i++)
	{		
		location[0][i] = delta.goalPos[i];			
	}
	if (LocationCompare_debug)
	{
		for (int i = 0; i < 3; i++)
		{
			My_Serial.print("location[0] -->");
			My_Serial.print(location[0][i]);
			My_Serial.print("\t");
		}
		My_Serial.print("\n");
		for (int i = 0; i < 3; i++)
		{
			My_Serial.print("location[1] -->");
			My_Serial.print(location[1][i]);
			My_Serial.print("\t");
		}
		My_Serial.print("\n");
	}	
	if (location[0] != location[1])
	{
		if (LocationCompare_debug)
		{
			My_Serial.println("!!  location is Different");
		}		
		for (int i = 0; i < 3; i++)
		{
			Slave_direction[i] = 2;
			degreeTurn[i] = location[0][i] - location[1][i];	
			if (LocationCompare_debug)
			{
				My_Serial.print("degreeTurn "); My_Serial.print(i); My_Serial.print(" = ");
				My_Serial.print(degreeTurn[i]); My_Serial.print("\t");
			}			
			if (degreeTurn[i] < 0)
			{
				if (LocationCompare_debug)
				{
					My_Serial.print("反向 \t");
				}				
				Slave_direction[i] = 1;
				degreeTurn[i] = (-1)*degreeTurn[i];
			}
		}		
		AfterCompare();
	}
	if (LocationCompare_debug)
	{
		My_Serial.println("\n**  LocationCompare --> END \n");
	}
}
//-------- LocationCompare -------------end ------------

//------turn-----
void turn()		//馬達轉動函式
{	
	//reset_delta();	
	turn_velocity(debug_off);
	turn_acc(debug_on);
	turn_degree(debug_on);
	turn_start();
}
//------turn-----end--------
//	>>>	turn velocity acc
void turn_velocity(boolean velocity_debug)	//contral_number	3
{
	if (velocity_debug)
	{
		MySerial.print(" ** turn_velocity --> Start \n");
	}
	unionType Slave;	
	contral_number = 3;
	Slave.b = Max_Speed;
	if (velocity_debug)
	{
		MySerial.print("Max_Speed \t");
		MySerial.print(Slave.b);
		MySerial.print("\n");
	}	
	for (int i = 0; i < 3; i++)
	{
		Wire.beginTransmission(MySlave1+i);
		Wire.write(contral_number);
		for (int i2 = 0; i2 < 4; i2++)
		{
			Wire.write(Slave.a[i2]);
			if (velocity_debug)
			{
				MySerial.print(Slave.a[i2]);
				MySerial.print("\t");
			}			
		}		
		Wire.endTransmission();
		if (velocity_debug)
		{			
			MySerial.print("\n");						
		}		
	}	
	if(velocity_debug)
	{
		MySerial.println(" ** turn_velocity	--> End \n");
	}	
}
//	<<<	turn velocity acc
void turn_acc(boolean turn_acc_debug)	//	contral_number	4
{
	if (turn_acc_debug)
	{
		MySerial.print("** turn_acc --> Start \n");		
	}
	unionType Slave;	
	contral_number = 4;
	double biggest = degreeTurn[0];
	int32_t newAcc[3];
	if (biggest < degreeTurn[1])
	{
		biggest = degreeTurn[1];
	}
	if (biggest < degreeTurn[2])
	{
		biggest = degreeTurn[2];
	}
	if (turn_acc_debug)
	{
		MySerial.print("biggest --> ");
		MySerial.println(biggest);
	}	
	for (int i = 0; i < 3; i++)
	{
		newAcc[i] = Add_Speed * (degreeTurn[i]/biggest);
		if (degreeTurn[i] == 0)
		{
			newAcc[i] = Add_Speed;
		}
		Slave.b = newAcc[i];
		if (turn_acc_debug)
		{
			MySerial.print("degreeTurn[i]/biggest = ");
			MySerial.print((double)(degreeTurn[i] / biggest));
			MySerial.print("\t");
			MySerial.print("New Acc --> ");
			MySerial.println(Slave.b);			
		}		
		Wire.beginTransmission(MySlave1 + i);
		Wire.write(contral_number);
		for (int i2 = 0; i2 < 4; i2++)
		{
			Wire.write(Slave.a[i2]);
			if (turn_acc_debug)
			{
				MySerial.print(Slave.a[i2]);
				MySerial.print("\t");
			}			
		}		
		Wire.endTransmission();
		if (turn_acc_debug)
		{			
			MySerial.print("\n");			
		}
	}
	if (turn_acc_debug)
	{
		MySerial.print("**  turn_acc --> End \n");
	}	
}
//	>>>	turn degree
void turn_degree(boolean turn_degree_debug)	//	contral_number		2	角度
{
	if (turn_degree_debug)
	{
		MySerial.print("\n**  turn_degree --> Start \n");
	}
	contral_number = 2;
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Slave1.b = degreeTurn[0] / microstep;
	Slave2.b = degreeTurn[1] / microstep;
	Slave3.b = degreeTurn[2] / microstep;	
	if (turn_degree_debug)
	{
		MySerial.print("Three  angles \n");
		MySerial.print(degreeTurn[0]); MySerial.print(" --> ");
		MySerial.println(Slave1.b);
		MySerial.print(degreeTurn[1]); MySerial.print(" --> ");
		MySerial.println(Slave2.b);
		MySerial.print(degreeTurn[2]); MySerial.print(" --> ");
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
	if (turn_degree_debug)
	{
		MySerial.print(" **  turn_degree --> End \n");
	}
}
//	<<<	turn degree	角度
//	>>>	turn start
void turn_start()	//	>>>	方向	contral_number 1
{
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	contral_number = 1;
	Slave1.b = Slave_direction[0];
	Slave2.b = Slave_direction[1];
	Slave3.b = Slave_direction[2];
	Wire.beginTransmission(MySlave1);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave1.a[i]);		
	}
	Wire.endTransmission();
	//
	Wire.beginTransmission(MySlave2);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave2.a[i]);		
	}
	Wire.endTransmission();
	//
	Wire.beginTransmission(MySlave3);
	Wire.write(contral_number);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave3.a[i]);		
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
//void PtoP_output()
//{
//	My_Serial.println();
//	My_Serial.println("PtoP_output");
//	LocationCompare();
//	My_Serial.print("三顆馬達轉動角度 = ");
//	for (int i = 0; i < 3; i++)
//	{
//		My_Serial.print(delta._theta2[i]);
//		if (i < 2)
//		{
//			My_Serial.print(" , ");
//		}
//	}
//	My_Serial.println();
//	My_Serial.print("三顆馬達轉動角度 2= ");
//	for (int i = 0; i < 3; i++)
//	{
//		My_Serial.print(D[i]);
//		if (i < 2)
//		{
//			My_Serial.print(" , ");
//		}
//	}
//	if (delta.maxArrIndex > 1)
//	{
//		My_Serial.println();
//		My_Serial.println(delta.maxArrIndex);
//		for (int i = 0; i < 3; i++)
//		{
//			My_Serial.println(location[delta.maxArrIndex - 2][i]);
//		};
//		My_Serial.print("兩次輸入度數差 = ");
//		for (int i = 0; i < 3; i++)
//		{
//			My_Serial.print(degreeTurn[i]);
//			if (i < 2)
//			{
//				My_Serial.print(" , ");
//			}
//		}
//	}
//
//}
//--------PtoP_output----------end-----

//	>>>	緊急停止相關
void E_Stop(boolean E_Stop_debug)
{
	contral_number = 6;
	if (strcmp(choose, "stop") == 0)
	{
		if (E_Stop_debug)
		{
			MySerial.println("** 緊急停止");
		}		
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

void suck_plate(boolean suck_plate_debug)
{
	if (suck_plate_debug)
	{		
		MySerial.println("beginTransmission(3)");
	}
	else;
	if (suck == 0)
	{
		if (suck_plate_debug)
		{
			MySerial.println("suck 0");
		}		
		Wire.beginTransmission(MySlave3);
		Wire.write("close");
		Wire.endTransmission();
	}
	else if (suck == 1)
	{
		if (suck_plate_debug)
		{
			MySerial.println("suck 1");
		}		
		Wire.beginTransmission(MySlave3);
		Wire.write("open");
		Wire.endTransmission();
	}
	else;
}

//	>>>	子版回傳資訊
void requestFrom()
{

}
//	<<<	子版回傳資訊

//	>>>	json相關
void json_reset()
{	
	n = 0;
	turn_json = 0;	
	if (choose != 0)
	{
		delete[] choose;
	}
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
	if (for_json_string_.string_turn_debug)
	{
		MySerial.println("forjson-buffer");
		MySerial.println(for_json_string_.json_string);
	}	
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(for_json_string_.json_string);
	if (!root.success())
	{
		if (for_json_string_.string_turn_debug)
		{
			MySerial.println("forjson-not-success");
		}		
		for_json_string_.String_reset();
		goto forjson_1;
	}	
	//	>>>	用char*陣列在某些MCU編譯時會有問題
	String json_string = root["func"];	
	choose = new char[json_string.length()];		
	for(int i=0;i<(json_string.length());i++)
	{
		choose[i] = json_string[i];
	}	
	choose[json_string.length()] = 0;
	n = root["n"];
	P[0] = root["p"][0];
	P[1] = root["p"][1];
	P[2] = root["p"][2];
	turn_json = root["turn"];
	//turn_json = (turn_json) / microstep;
	Max_Speed = root["pps"];
	Add_Speed = root["acc"];
	if (for_json_string_.string_turn_debug)
	{
		MySerial.println(choose);
		MySerial.println(turn_json);
		MySerial.println(Max_Speed);
		MySerial.println(Add_Speed);
		MySerial.print("\n");
	}	
	while (MySerial.available() > 0)
	{
		MySerial.read();
	}
}
//	<<<	Forjson
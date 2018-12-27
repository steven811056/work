//正在修改scara2.0版本第2節兩段式移動
//吸盤轉動角度以經修改完成
//wait for slave 等待子版回復 做到一半
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Stop.h>
#include "math.h"
#include <Wire.h>
#include <arduino.h>
//#include "MifesSCB.h"

#define Microstep_control 0.1	//微步控制倍率	16
//#define Microstep_control 	//微步控制倍率	8
//#define Microstep_control 	//微步控制倍率	4

#define first_arm 10	//第一節臂參數
#define second_arm 10	//第二節臂參數
#define Sprint(a) MySerial.print(a)
#define debug 1
#define MySerial Serial
#define slave1	0x01
#define slave2	0x02
#define slave3	0x03
#define slave4	0x04
//  slave 1 是控制AB節臂的馬達   slave 2 是控制BC節臂的馬達  slave 3 是控制Z軸的馬達
//---josn
char json[200];
int jsonNumber = 0;
double P[3] = { 0, 0, 0 };  //本次輸入的位置
char* choose;	//選擇輸入模式
int suck;		//吸盤
int suck_turn;	//吸盤轉動角度
//---
//---比對
int16_t AddressGap[3];  //輸入與前一次所在的距離差
double P2[3] = { 0,0,0 };	//上次的位置
//----
int8_t Max = 0;  //運轉圈數
uint16_t t;  //運轉時間
//---演算法相關
double L_AB;  //A到B點的距離
double F_CD; //CD直線的斜率
double L_EF, L_AP, L_BP;
double L_AG, L_CG, L_AE, L_BF;
double C_X1, C_X2, C_Y1, C_Y2;
double thetalOne, thetalTwo;  //角BAC 與 直線AC與X的夾角
double thetalThree, thetalFour, thetalTF;
double thetal_A[2] = { 0,0 };  //A所要轉的方向跟角度
double thetal_A2[2] = { 0,0 };	//A上一次所轉的方向跟角度
double thetal_A3[2] = { 0,0 };	//存放這次減掉上次的值
double thetal_B[2] = { 0,0 };	//B所要轉的方向跟角度
double thetal_B2[2] = { 0,0 };	//B上一次所轉的方向跟角度
double thetal_B3[2] = { 0,0 };	//存放這次減掉上次的值 輸出給slave
double thetal_B4[2] = { 0,0 };
double thetal_B_C = 0;	//存放這次減掉上次的值 拿來比較用compare
double thetal_Z[2] = { 0,0 };	 //Z所要轉的方向跟角度
double thetal_Z2[2] = { 0,0 };	//Z上一次所轉的方向跟角度
double thetal_Z3[2] = { 0,0 };	//存放這次減掉上次的值
double turn;  //y比x的比值		
//-----

union unionType
{
	byte a[8] = { 0,0,0,0,0,0,0,0 };
	int32_t b;
};
union unionType2
{
	byte a[2] = { 0,0 };
	int16_t b;
};
//EStop Emergency_Stop;
void setup()
{
	Wire.begin();
	MySerial.begin(115200);	
	if (debug)
	{
		MySerial.print("目前初始位置:(");
		for (int i = 0; i < 3; i++)
		{
			MySerial.print(P[i]);
			if (i < 2)
				MySerial.print(",");
		}
		MySerial.println(")");
		MySerial.println();
	}	
	choose = "!";
}

void loop()
{
	if (choose == "!")
	{
		MySerial.println("輸入指令");
		for (int i = 0; i < sizeof(json); i++)
		{
			json[i] = ' ';
		}
		ForJson();
		if (debug)
		{
			MySerial.println("JSON -->  OK");
		}		
	}
	while (1)
	{		
		if (strcmp(choose, "0") == 0)
		{
			scara_reset();
			wait_for_slave();
			break;
		}
		else if (strcmp(choose, "1") == 0) //1號模式 輸入座標
		{
			if (debug)
			{
				MySerial.println("座標模式");
			}			
			ctrl_deg();
			choose = "!";
			if (debug)
			{
				MySerial.println();
			}					
			break;
		}
		else if (strcmp(choose, "2") == 0)  //2號模式 輸入角度
		{
			if (debug)
			{
				MySerial.println("角度模式");
				MySerial.println("開始與slave通訊");
			}			
			connect();
			if (debug)
			{
				MySerial.println("結束與slave通訊");
				MySerial.println();
			}	
			choose = "!";	
			break;
		}
		else if (strcmp(choose, "3") == 0)
		{
			unionType Slave4;			
			if (debug)
			{
				MySerial.println("夾爪控制");
			}
			else{};	
			if (suck == 0)
			{
				Wire.beginTransmission(slave4);
				Wire.write("close");
				Wire.endTransmission();
			}
			else if (suck == 1)
			{
				Wire.beginTransmission(slave4);
				Wire.write("open");
				Wire.endTransmission();
			}			
			else if (suck == 2)
			{
				Wire.beginTransmission(slave4);
				Wire.write("turn");
				Wire.endTransmission();		
				delay(1);
				if (suck_turn >= 0)
				{
					Slave4.b = (suck_turn / Microstep_control);
					if (debug)
					{
						MySerial.println(">0");
					}
					Wire.beginTransmission(slave4);
					Wire.write("start");
					Wire.endTransmission();
					delay(10);
					Wire.beginTransmission(slave4);
					for (int i = 0; i < 4; i++)
					{
						if (Slave4.a[i] == 0)
						{
							break;
						}
					Wire.write(Slave4.a[i]);
					MySerial.println(Slave4.a[i]);
					}
					Wire.endTransmission();					
				}
				else if (suck_turn < 0)
				{
					Slave4.b = (suck_turn*(-1) / Microstep_control);
					if (debug)
					{
						MySerial.println("<0");
					}
					Wire.beginTransmission(slave4);
					Wire.write("start2");
					Wire.endTransmission();
					delay(10);
					Wire.beginTransmission(slave4);
					for (int i = 0; i < 4; i++)
					{
						if (Slave4.a[i] == 0)
						{
							break;
						}
						Wire.write(Slave4.a[i]);
						MySerial.println(Slave4.a[i]);
					}
					Wire.endTransmission();
				}
			}
			choose = "!";
			break;
		}
	}
}

//----------------座標輸入---------------
void ctrl_deg()
{
	L_AB = 0;  //A到B點的距離
	F_CD = 0; //CD直線的斜率
	L_EF = 0;
	L_AP = 0;
	L_BP = 0;
	L_AG = 0, L_CG = 0, L_AE = 0, L_BF = 0;
	thetalOne = 0, thetalTwo = 0;  //角BAC 與 直線AC與X的夾角
	thetalThree = 0, thetalFour = 0, thetalTF = 0;	
	if (debug)
	{
		MySerial.println("座標輸入完成--開始運算");
	}	
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (L_AB>20 || L_AB<11 || L_AB ==0)
	{
		if (debug)
		{
			MySerial.println("輸入錯誤，重新輸入");
		}
		for (int i = 0; i < sizeof(json); i++)
		{
			json[i] = ' ';
		}
		ForJson();
	}
	/*if (L_AB<21 && L_AB>10)*/
	else
	{
		if (debug)
		{
			Quadrant_Judge();
		}		
		Scara_3axis();
		choose = "!";
	}
	if (debug)
	{
		MySerial.println();
	}	
}
//********----------------座標輸入---------------

//----------象限判斷-----------
void Quadrant_Judge()
{
	if (P[1]>0)
	{
		if (P[0]>0)
		{
			MySerial.println("第一象限");
		}
		if (P[0]<0)
		{
			MySerial.println("第二象限");
		}
	}
}
//*******----------象限判斷-----------

//----------座標角度轉換------------
void Scara_3axis()
{
	Max++;	
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;	
	for (int i = 0; i < 2; i++)
	{
		thetal_A2[i] = thetal_A[i];
		thetal_B2[i] = thetal_B[i];
		thetal_Z2[i] = thetal_Z[i];
	}	
	if (debug)
	{
		MySerial.print("運轉圈數 --> ");
		MySerial.println(Max);
	}
	else;
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (debug)
	{
		MySerial.print("L_AB -> ");
		MySerial.println(L_AB);
	}
	else;
	L_CG = L_AG - first_arm;
	L_AE = L_AB - second_arm;
	L_BF = L_AB - first_arm;
	L_EF = L_AB - L_AE - L_BF;
	L_AP = (L_EF / 2) + L_AE;
	L_BP = (L_EF / 2) + L_BF;
	turn = abs(P[1] / P[0]);
	if (debug)
	{
		MySerial.println(cos(abs(P[1]) / L_AB));
		MySerial.print("直線方程式 -> ");
		MySerial.print("y = ");	MySerial.print(double(P[1]) / double(P[0]));	MySerial.println("X");
		MySerial.print("垂直於方程式的直線 -> ");
		MySerial.print("y = ");	MySerial.print((double(P[0]) / double(P[1]))*(-1));	MySerial.println("X");
	}	
	F_CD = (double(P[0]) / double(P[1]))*(-1);
	if (debug)
	{
		MySerial.print("F_CD 斜率 -> ");
		MySerial.println(F_CD);
		//---
		MySerial.print("X   --> ");
		MySerial.print(C_X1);
		MySerial.print("\t");
		MySerial.println(C_X2);
		//--
	}	
	thetalOne = acos(L_AP / first_arm) * 180 / PI;
	if (debug)
	{
		MySerial.print("thetalOne  turn -->");
		MySerial.println(cos(L_AP / first_arm));
		MySerial.print("thetalOne  turn -->");
		MySerial.println(thetalOne);
	}	
	//---
	thetalTwo = atan(turn) * 180 / PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
	if (debug)
	{
		MySerial.print("thetalTwo  turn -->");
		MySerial.println(cos(abs(P[1]) / L_AB));
		MySerial.print("thetalTwo  turn -->");
		MySerial.println(thetalTwo);
	}	
	//---	thetalThree -> 第一軸與原點、L_AP的夾角
	thetalThree = asin(L_AP / first_arm) * 180 / PI;
	if (debug)
	{
		MySerial.print("thetalThree  turn -->");
		MySerial.println(thetalThree);
	}	
	//--		thetalFour -> 第一軸與終點、L_AP的夾角
	thetalFour = asin(L_BP / second_arm) * 180 / PI;
	if (debug)
	{
		MySerial.print("thetalFour  turn -->");
		MySerial.println(thetalFour);
	}	
	thetalTF = 180 - thetalThree - thetalFour;	//	第2節所要轉動的角度
	thetal_A[1] = thetalOne + thetalTwo;	//第一節所要轉的角度
	if (P[0] < 0)	
	{	//從另一象限看
		thetal_A[1] = 180 - thetalTwo - thetalOne;
	}
	if (debug)
	{
		MySerial.print("thetal_A轉動");
		MySerial.print(thetal_A[1]);
		MySerial.println("度");
	}	
	//----
	thetal_Z[1] = P[2];	//	Z軸所要轉的高度
	thetal_B[1] = thetalTF;	//	第2節所要轉動的角度
	//	這次算的角度減掉上次算的角度
	thetal_A3[1] = thetal_A[1] - thetal_A2[1];		
	thetal_B_C= thetal_B[1] - thetal_B2[1];	
	/*if (thetal_B2[0] != thetal_B[0])
	{	//	決定thetal_B3的值
		thetal_B3[1] = 
	}*/
	if ( (thetal_B_C) == 0)
	{
		thetal_B3[1] = 0;
	}
	else
	{
		thetal_B3[1] = thetal_B[1];
	}	
	//	
	thetal_Z3[1] = thetal_Z[1] - thetal_Z2[1];
	delay(1);	
	//
	F_B_Ward();
	//
	if (debug)
	{
		MySerial.print("輸出的3個角度 --> ");
		MySerial.print(thetal_A3[0]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_A3[1]);
		MySerial.println();
		MySerial.print(thetal_B3[0]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_B3[1]);		
		MySerial.println();
		MySerial.print(thetal_Z3[0]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_Z3[1]);
		MySerial.println();
	}	
	//-----****	
	//---------
	if (Max > 1)
	{
		if (thetal_B_C != 0)
		{
			if (debug)
			{
				MySerial.print("B返回 --> ");
				MySerial.println(thetal_B2[1]);
				MySerial.println((thetal_B2[1] * 10) / (0.05625 * 2));
			}						
		}
		else {};
	}		
	//----****	把計算完成的角度存入要傳送給slave的變數
	thetal_Z[1] = P[2];
	thetal_B[1] = thetalTF;	
	Slave1.b = (thetal_A3[1] * 20) / (0.05625 * 2);
	if (debug)
	{
		Serial.print("Slave1.b = ");
		Serial.println(Slave1.b);
	}
	Slave2.b = (thetal_B3[1] * 10) / (0.05625 * 2);
	if (debug)
	{
		Serial.print("Slave2.b = ");
		Serial.println(Slave2.b);
	}
	if (thetal_Z3[1] == 0)
	{
		Slave3.b = 0;
	}
	else 
	{
		Slave3.b = (thetal_Z3[1] * 3200 + 320);
	}
	
	//---**
	if (debug)
	{
		MySerial.println("beginTransmission(slave3)");
	}
	Wire.beginTransmission(slave3);
	for (int i = 0; i < 4; i++)
	{		
		Wire.write(Slave3.a[i]);
		MySerial.println(Slave3.a[i]);
	}
	Wire.endTransmission();
	delay(200);
	if (debug)
	{
		MySerial.println("beginTransmission(slave1)");
	}	
	Wire.beginTransmission(slave1);	
	for (int i = 0; i < 4; i++)
	{			
		Wire.write(Slave1.a[i]);
		MySerial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		MySerial.println("beginTransmission(slave2)");
	}
	Wire.beginTransmission(slave2);	
	for (int i = 0; i < 4; i++)
	{
		/*if (Slave2.a[i] == 0)
		{
			break;
		}*/
		Wire.write(Slave2.a[i]);
		MySerial.println(Slave2.a[i]);
	}
	Wire.endTransmission();	
	if (debug)
	{
		MySerial.print("thetal_A--byte -> ");
		MySerial.println(thetal_A[1]);
		MySerial.print("thetal_B轉動 -> ");
		MySerial.print(thetal_B[1]);
		MySerial.println("度");
		MySerial.print("Z軸旋轉 -> ");
		MySerial.println(P[2]);
	}
	for (int i = 0; i < 2; i++)
	{
		P2[i] = P[i];
	}
	wait_for_slave();
}
//************---------------座標角度轉換------------

//----判斷正反轉----
void F_B_Ward()
{
	//	決定第1節的正反轉
	if (thetal_A3[1] < 0)
	{
		thetal_A3[0] = 1;
		thetal_A3[1] = -(thetal_A3[1]);
	}
	else
	{
		thetal_A3[0] = 0;
	}
	//	決定Z軸的正反轉
	if (thetal_Z3[1] < 0)
	{
		thetal_Z3[0] = 1;
		thetal_Z3[1] = -(thetal_Z3[1]);
	}
	else
	{
		thetal_Z3[0] = 0;
	}
	//	決定第2節的正反轉
	if (thetal_B3[1] < 0)
	{
		thetal_B3[0] = 1;
		thetal_B3[1] = -(thetal_B3[1]);
	}
	else
	{
		thetal_B3[0] = 0;
	}
	//--		判斷第二節正反轉	
	if (P[0] >= 0)
	{
		thetal_B3[0] = 0;
		if( ( thetal_B2[1] - thetal_B[1] ) >=0)
		Wire.beginTransmission(slave2);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		thetal_B3[0] = 1;
		Wire.beginTransmission(slave2);
		Wire.write("start2");
		Wire.endTransmission();
	}
	//--		判斷第一節與Z軸正反轉
	if (thetal_A3[0] == 0)
	{
		if (debug)
		{
			MySerial.println(" = 0");
		}
		else;
		Wire.beginTransmission(slave1);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			MySerial.println(" != 0");
		}
		Wire.beginTransmission(slave1);
		Wire.write("start2");
		Wire.endTransmission();
	}
	if (thetal_Z3[0] == 0)
	{
		if (debug)
		{
			MySerial.println(" = 0");
		}
		Wire.beginTransmission(slave3);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			MySerial.println(" != 0");
		}
		Wire.beginTransmission(slave3);
		Wire.write("start2");
		Wire.endTransmission();
	}
}
//----******判斷正反轉----

//-----等待子版回復-----
void wait_for_slave()
{
	if (debug)
	{
		MySerial.println("WAIT");
	}
	int a = 0;
	while (1)
	{		
		Wire.requestFrom(slave1,4);
		if (Wire.available())
		{
			a = a + Wire.read();
			/*if (debug)
			{
				MySerial.println(a);
			}*/
		}
		else;
		if (a == 1)
		{
			MySerial.println("OK");
			break;
		}
		delay(50);
	}
}
//-----********等待子版回復-----

//---------度數直接控制-----------
void connect()
{
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Wire.beginTransmission(slave1);
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(slave2);
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(slave3);
	Wire.write("start");
	Wire.endTransmission();
	MySerial.println();
	MySerial.println("輸入三軸角度");
	while (1)
	{
		if (MySerial.available())
		{
			P[0] = MySerial.parseInt();
			P[1] = MySerial.parseInt();
			P[2] = MySerial.parseInt();
			break;
		}
	}
	Slave1.b = P[0];
	Slave2.b = P[1];
	Slave3.b = P[2];
	MySerial.println(Slave1.b);
	MySerial.println(Slave2.b);
	MySerial.println(Slave3.b);
	MySerial.println();
	Wire.beginTransmission(slave1);
	MySerial.println("beginTransmission(slave1)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] != 0)
		{
			Wire.write(Slave1.a[i]);
			MySerial.println(Slave1.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(slave2);
	MySerial.println("beginTransmission(slave2)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] != 0)
		{
			Wire.write(Slave2.a[i]);
			MySerial.println(Slave2.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(slave3);
	MySerial.println("beginTransmission(slave3)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave3.a[i] != 0)
		{
			Wire.write(Slave3.a[i]);
			MySerial.println(Slave3.a[i]);
		}
	}
	Wire.endTransmission();
}
//**********---------度數直接控制-----------

//-----------歸零------------
void scara_reset()
{	/*  歸零方式我定為AB臂轉180度壓住極限開關
	  ，BC臂為壓到極限開關後往反方向旋轉135度  
	  Wire.beginTransmission(1);
	    Wire.write(33);  
	  ascii碼傳輸!  之前用3沒有顯是是因為ascii碼的3是^C的意思
	  Wire.endTransmission();
	  for (int i = 0; i<2; i++)
	  {
	  thetalOne = 0;
	  thetalTwo = 0;
	  thetal_A[i] = 0;
	  thetal_B[i] = 0;
	  }*/
	if (debug)
	{
		MySerial.println("reset");
	}		
	Wire.beginTransmission(slave3);
	Wire.write("reset");
	Wire.endTransmission();	
	Wire.beginTransmission(slave1);
	Wire.write("reset");
	Wire.endTransmission();	
	Wire.beginTransmission(slave2);
	Wire.write("reset");
	Wire.endTransmission();	
	if (debug)
	{
		MySerial.println("reset END");
	}	
	for (int i = 0; i < 3; i++)
	{
		P[i] = 0;
		P2[i] = 0;
	}		
	for (int i = 0; i < 2; i++)
	{
		thetal_A[i] = 0;
		thetal_A2[i] = 0;
		thetal_A3[i] = 0;
		thetal_B[i] = 0;
		thetal_B2[i] =0;
		thetal_B3[i] = 0;
		thetal_B_C = 0;
		thetal_Z[i] = 0;
		thetal_Z2[i] = 0;
		thetal_Z3[i] = 0;
	}
	suck = 0 ;	//吸盤
	suck_turn = 0;
	turn = 0;	
	Max = 0;
	choose = "!";
}
//***********-----------歸零------------

//--------json字串讀取---------
void ForJson()
{
	jsonNumber = 0;
	int a = 0;
	choose = "";
	P[0] = 0;
	P[1] = 0;
	P[2] = 0;
	suck = 0;
	suck_turn = 0;
	while (1)
	{
		if (MySerial.available())
		{
			json[jsonNumber] = MySerial.read();
			if (json[jsonNumber] == '{')
			{
				a++;
			}

			if (json[jsonNumber] == '}')
			{
				a++;
			}
			jsonNumber++;
		}

		if (a == 2)
		{
			MySerial.println(json);
			StaticJsonBuffer<200> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			choose = root["choose"];
			P[0] = root["X"];
			P[1] = root["Y"];
			P[2] = root["Z"];
			suck = root["suck"];
			suck_turn = root["suck_turn"];
			if (debug)
			{
				MySerial.println(choose);
				MySerial.println(P[0]);
				MySerial.println(P[1]);
				MySerial.println(P[2]);
				MySerial.println(suck);
				MySerial.println(suck_turn);
			}			
			a = 0;			
			break;
		}
	}
}
//--------********json字串讀取---------

#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Stop.h>
#include "math.h"
#include <Wire.h>
#include <arduino.h>
//#include "MifesSCB.h"

#define first_arm 10	//第一節臂參數
#define second_arm 10	//第二節臂參數
#define Sprint(a) My_Serial.print(a)
#define debug 1
#define My_Serial Serial
//  slave 1 是控制AB節臂的馬達   slave 2 是控制BC節臂的馬達  slave 3 是控制Z軸的馬達
//---
const int stepPin[3] = { 2, 4, 6 };
const int dirPin[3] = { 3, 5, 7 };
//---josn
char json[200];
String jsonS;
char jsonC;
int jsonNumber = 0;
//---
//int16_t Address[100][3];  //輸入位址的紀錄存放
int16_t AddressGap[3];  //輸入與前一次所在的距離差

int8_t Max = 0;  //運轉圈數
double P[3] = { 0, 0, 0 };  //本次輸入的位置
double P2[3] = { 0,0,0 };	//上次的位置
uint16_t t;  //運轉時間
	     //char choose = '!';  //選擇輸入模式
char* choose;
int suck;		//吸盤
double L_AB;  //A到B點的距離
double F_CD; //CD直線的斜率
double L_EF, L_AP, L_BP;
double L_AG, L_CG, L_AE, L_BF;
double C_X1, C_X2, C_Y1, C_Y2;
double thetalOne, thetalTwo;  //角BAC 與 直線AC與X的夾角
double thetalThree, thetalFour, thetalTF;
double thetal_A[2] = { 0,0 };  //A所要轉的方向跟角度
double thetal_A2[2] = { 0,0 };
double thetal_A3[2] = { 0,0 };
double thetal_B[2] = { 0,0 }; //B所要轉的方向跟角度
double thetal_B2[2] = { 0,0 };
double thetal_B3[2] = { 0,0 };
double thetal_B4 = 0;
double thetal_Z[2] = { 0,0 };
double thetal_Z2[2] = { 0,0 };
double thetal_Z3[2] = { 0,0 };
double turn;  //y比x的比值		

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
	My_Serial.begin(9600);
	/*for (int i = 0; i < 3; i++)
	{
	pinMode(stepPin[i], OUTPUT);
	pinMode(dirPin[i], OUTPUT);
	}*/

	if (debug)
	{
		My_Serial.print("目前初始位置:(");
		for (int i = 0; i < 3; i++)
		{
			My_Serial.print(P[i]);
			if (i < 2)
				My_Serial.print(",");
		}
		My_Serial.println(")");
		My_Serial.println();
	}
	choose = "!";
}

void loop()
{
	if (choose == "!")
	{
		My_Serial.println("輸入指令");
		for (int i = 0; i < sizeof(json); i++)
		{
			json[i] = ' ';
		}
		ForJson();
		if (debug)
		{
			My_Serial.println("JSON -->  OK");
		}
	}
	while (1)
	{
		if (strcmp(choose, "0") == 0)
		{
			scara_reset();

			break;
		}
		else if (strcmp(choose, "1") == 0) //1號模式 輸入座標
		{
			if (debug)
			{
				My_Serial.println("座標模式");
			}
			ctrl_deg();
			choose = "!";
			if (debug)
			{
				My_Serial.println();
			}
			break;
		}
		else if (strcmp(choose, "2") == 0)  //2號模式 輸入角度
		{
			if (debug)
			{
				My_Serial.println("角度模式");
				My_Serial.println("開始與slave通訊");
			}
			connect();
			if (debug)
			{
				My_Serial.println("結束與slave通訊");
				My_Serial.println();
			}
			choose = "!";
			break;
		}
		else if (strcmp(choose, "3") == 0)
		{
			if (debug)
			{
				My_Serial.println("夾爪控制");
			}
			else {};
			if (suck == 2)
			{
				Wire.beginTransmission(4);
				Wire.write("turn");
				Wire.endTransmission();
			}
			else if (suck == 1)
			{
				Wire.beginTransmission(4);
				Wire.write("open");
				Wire.endTransmission();
			}
			else if (suck == 0)
			{
				Wire.beginTransmission(4);
				Wire.write("close");
				Wire.endTransmission();
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
		My_Serial.println("座標輸入完成--開始運算");
	}
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (L_AB>20 || L_AB<11 || L_AB == 0)
	{
		if (debug)
		{
			My_Serial.println("輸入錯誤，重新輸入");
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
		delta_3axis();
		choose = "!";
	}
	if (debug)
	{
		My_Serial.println();
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
			My_Serial.println("第一象限");
		}
		if (P[0]<0)
		{
			My_Serial.println("第二象限");
		}
	}
}
//*******----------象限判斷-----------

//----------座標角度轉換------------
void delta_3axis()
{
	Max++;
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	unionType Slave2_1;
	for (int i = 0; i < 2; i++)
	{
		thetal_A2[i] = thetal_A[i];
		thetal_B2[i] = thetal_B[i];
		thetal_Z2[i] = thetal_Z[i];
	}
	if (debug)
	{
		My_Serial.print("運轉圈數 --> ");
		My_Serial.println(Max);
	}
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (debug)
	{
		My_Serial.print("L_AB -> ");
		My_Serial.println(L_AB);
	}
	L_CG = L_AG - first_arm;
	L_AE = L_AB - second_arm;
	L_BF = L_AB - first_arm;
	L_EF = L_AB - L_AE - L_BF;
	L_AP = (L_EF / 2) + L_AE;
	L_BP = (L_EF / 2) + L_BF;
	turn = abs(P[1] / P[0]);
	if (debug)
	{
		My_Serial.println(cos(abs(P[1]) / L_AB));
		My_Serial.print("直線方程式 -> ");
		My_Serial.print("y = ");	My_Serial.print(double(P[1]) / double(P[0]));	My_Serial.println("X");
		My_Serial.print("垂直於方程式的直線 -> ");
		My_Serial.print("y = ");	My_Serial.print((double(P[0]) / double(P[1]))*(-1));	My_Serial.println("X");
	}
	F_CD = (double(P[0]) / double(P[1]))*(-1);
	if (debug)
	{
		My_Serial.print("F_CD 斜率 -> ");
		My_Serial.println(F_CD);
		//---
		My_Serial.print("X   --> ");
		My_Serial.print(C_X1);
		My_Serial.print("\t");
		My_Serial.println(C_X2);
		//--
	}
	thetalOne = acos(L_AP / first_arm) * 180 / PI;
	if (debug)
	{
		My_Serial.print("thetalOne  turn -->");
		My_Serial.println(cos(L_AP / first_arm));
		My_Serial.print("thetalOne  turn -->");
		My_Serial.println(thetalOne);
	}
	//---
	thetalTwo = atan(turn) * 180 / PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
	if (debug)
	{
		My_Serial.print("thetalTwo  turn -->");
		My_Serial.println(cos(abs(P[1]) / L_AB));
		My_Serial.print("thetalTwo  turn -->");
		My_Serial.println(thetalTwo);
	}
	//---
	thetalThree = asin(L_AP / first_arm) * 180 / PI;
	if (debug)
	{
		My_Serial.print("thetalThree  turn -->");
		My_Serial.println(thetalThree);
	}
	//--
	thetalFour = asin(L_BP / second_arm) * 180 / PI;
	if (debug)
	{
		My_Serial.print("thetalFour  turn -->");
		My_Serial.println(thetalFour);
	}
	thetalTF = 180 - thetalThree - thetalFour;
	thetal_A[1] = thetalOne + thetalTwo;
	if (P[0] < 0)
	{
		thetal_A[1] = 180 - thetalTwo - thetalOne;
	}
	if (debug)
	{
		My_Serial.print("thetal_A轉動");
		My_Serial.print(thetal_A[1]);
		My_Serial.println("度");
	}
	//----
	thetal_Z[1] = P[2];
	thetal_B[1] = thetalTF;
	thetal_A3[1] = thetal_A[1] - thetal_A2[1];
	thetal_B4 = thetal_B[1] - thetal_B2[1];//**
	if (thetal_B4 == 0)
	{
		thetal_B3[1] = 0;
	}
	else
	{
		thetal_B3[1] = thetal_B[1];//**
	}
	thetal_Z3[1] = thetal_Z[1] - thetal_Z2[1];
	delay(1);
	if (thetal_A3[1] < 0)
	{
		thetal_A3[0] = 1;
		thetal_A3[1] = -(thetal_A3[1]);
	}
	else
	{
		thetal_A3[0] = 0;
	}
	//
	if (thetal_B3[1] < 0)
	{
		thetal_B3[0] = 1;
		thetal_B3[1] = -(thetal_B3[1]);
	}
	else
	{
		thetal_B3[0] = 0;
	}
	//
	if (thetal_Z3[1] < 0)
	{
		thetal_Z3[0] = 1;
		thetal_Z3[1] = -(thetal_Z3[1]);
	}
	else
	{
		thetal_Z3[0] = 0;
	}
	if (debug)
	{
		My_Serial.print("輸出的3個角度 --> ");
		My_Serial.print(thetal_A3[0]);
		My_Serial.print(" <---> ");
		My_Serial.print(thetal_A3[1]);
		My_Serial.println();
		My_Serial.print(thetal_B3[0]);
		My_Serial.print(" <---> ");
		My_Serial.print(thetal_B3[1]);
		My_Serial.print(" <---> ");
		My_Serial.print(thetal_B4);
		My_Serial.println();
		My_Serial.print(thetal_Z3[0]);
		My_Serial.print(" <---> ");
		My_Serial.print(thetal_Z3[1]);
		My_Serial.println();
	}
	//-----****	
	//---------
	if (Max > 1)
	{
		if (thetal_B4 != 0)
		{
			if (debug)
			{
				My_Serial.print("B返回 --> ");
				My_Serial.println(thetal_B2[1]);
				My_Serial.println((thetal_B2[1] * 10) / (0.05625 * 2));
			}
			Slave2_1.b = (thetal_B2[1] * 10) / (0.05625 * 2);
			if (P2[0] >= 0)
			{
				Wire.beginTransmission(0x02);
				Wire.write("start2");
				Wire.endTransmission();
				delay(10);
				Wire.beginTransmission(0x02);
				for (int i = 0; i < 4; i++)
				{
					if (Slave2_1.a[i] == 0)
					{
						break;
					}
					Wire.write(Slave2_1.a[i]);
					My_Serial.println(Slave2_1.a[i]);
				}
				Wire.endTransmission();
			}
			else
			{
				Wire.beginTransmission(0x02);
				Wire.write("start");
				Wire.endTransmission();
				delay(10);
				Wire.beginTransmission(0x02);
				for (int i = 0; i < 4; i++)
				{
					if (Slave2_1.a[i] == 0)
					{
						break;
					}
					Wire.write(Slave2_1.a[i]);
					My_Serial.println(Slave2_1.a[i]);
				}
				Wire.endTransmission();
			}
			delay(2000);
		}
		else {};
	}
	//----****
	thetal_Z[1] = P[2];
	thetal_B[1] = thetalTF;
	/*Slave1.b = (thetal_A[1] * 20) / (0.05625 * 2);
	Slave2.b = (thetal_B[1] * 10) / (0.05625 * 2);
	Slave3.b = (thetal_Z[1] * 3200 + 320);	*/

	Slave1.b = (thetal_A3[1] * 20) / (0.05625 * 2);
	Slave2.b = (thetal_B3[1] * 10) / (0.05625 * 2);
	if (thetal_Z3[1] == 0)
	{
		Slave3.b = 0;
	}
	else
	{
		Slave3.b = (thetal_Z3[1] * 3200 + 320);
	}
	//--			
	if (P[0] >= 0)
	{
		Wire.beginTransmission(0x02);
		Wire.write("start");
		Wire.endTransmission();
	}
	if (P[0] < 0)
	{
		Wire.beginTransmission(0x02);
		Wire.write("start2");
		Wire.endTransmission();
	}

	//--		
	if (thetal_A3[0] == 0)
	{
		if (debug)
		{
			My_Serial.println(" = 0");
		}
		Wire.beginTransmission(0x01);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			My_Serial.println(" != 0");
		}
		Wire.beginTransmission(0x01);
		Wire.write("start2");
		Wire.endTransmission();
	}
	if (thetal_Z3[0] == 0)
	{
		if (debug)
		{
			My_Serial.println(" = 0");
		}
		Wire.beginTransmission(0x03);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			My_Serial.println(" != 0");
		}
		Wire.beginTransmission(0x03);
		Wire.write("start2");
		Wire.endTransmission();
	}
	//---**
	if (debug)
	{
		My_Serial.println("beginTransmission(0x03)");
	}
	Wire.beginTransmission(0x03);
	for (int i = 0; i < 4; i++)
	{
		if (Slave3.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave3.a[i]);
		My_Serial.println(Slave3.a[i]);
	}
	Wire.endTransmission();
	delay(200);
	if (debug)
	{
		My_Serial.println("beginTransmission(0x01)");
	}
	Wire.beginTransmission(0x01);
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave1.a[i]);
		My_Serial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println("beginTransmission(0x02)");
	}
	Wire.beginTransmission(0x02);
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] == 0)
		{
			break;
		}
		Wire.write(Slave2.a[i]);
		My_Serial.println(Slave2.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.print("thetal_A--byte -> ");
		My_Serial.println((byte)thetal_A[1]);
		My_Serial.print("thetal_B轉動 -> ");
		My_Serial.print(thetal_B[1]);
		My_Serial.println("度");
		My_Serial.print("Z軸旋轉 -> ");
		My_Serial.println(P[2]);
	}
	for (int i = 0; i < 2; i++)
	{
		P2[i] = P[i];
	}
}
//************---------------座標角度轉換------------

//---------度數直接控制-----------
void connect()
{
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Wire.beginTransmission(0x01);
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(0x02);
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(0x03);
	Wire.write("start");
	Wire.endTransmission();
	My_Serial.println();
	My_Serial.println("輸入三軸角度");
	while (1)
	{
		if (My_Serial.available())
		{
			P[0] = My_Serial.parseInt();
			P[1] = My_Serial.parseInt();
			P[2] = My_Serial.parseInt();
			break;
		}
	}
	Slave1.b = P[0];
	Slave2.b = P[1];
	Slave3.b = P[2];
	My_Serial.println(Slave1.b);
	My_Serial.println(Slave2.b);
	My_Serial.println(Slave3.b);
	My_Serial.println();
	Wire.beginTransmission(0x01);
	My_Serial.println("beginTransmission(0x01)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] != 0)
		{
			Wire.write(Slave1.a[i]);
			My_Serial.println(Slave1.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x02);
	My_Serial.println("beginTransmission(0x02)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] != 0)
		{
			Wire.write(Slave2.a[i]);
			My_Serial.println(Slave2.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x03);
	My_Serial.println("beginTransmission(0x03)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave3.a[i] != 0)
		{
			Wire.write(Slave3.a[i]);
			My_Serial.println(Slave3.a[i]);
		}
	}
	Wire.endTransmission();
}
//**********---------度數直接控制-----------

//-----------歸零------------
void scara_reset()
{ /*  歸零方式我定為AB臂轉180度壓住極限開關
  ，BC臂為壓到極限開關後往反方向旋轉135度  */
  //Wire.beginTransmission(1);
  //  Wire.write(33);  
  //ascii碼傳輸!  之前用3沒有顯是是因為ascii碼的3是^C的意思
  //Wire.endTransmission();
  /*for (int i = 0; i<2; i++)
  {
  thetalOne = 0;
  thetalTwo = 0;
  thetal_A[i] = 0;
  thetal_B[i] = 0;
  }*/
	if (debug)
	{
		My_Serial.println("reset");
	}
	Wire.beginTransmission(0x03);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(0x01);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(0x02);
	Wire.write("reset");
	Wire.endTransmission();
	if (debug)
	{
		My_Serial.println("reset END");
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
		thetal_B2[i] = 0;
		thetal_B3[i] = 0;
		thetal_B4 = 0;
		thetal_Z[i] = 0;
		thetal_Z2[i] = 0;
		thetal_Z3[i] = 0;
	}
	suck = 0;	//吸盤
	turn = 0;
	Max = 0;
	choose = "!";
}
//***********-----------歸零------------

//------Uart---------
void For_Uart()
{
	jsonC = ' ';
	int a = 0;
	while (1)
	{
		while (Serial.available())
		{
			jsonC = Serial.read();
			jsonS = jsonS + jsonC;
			if (jsonC == '{')
			{
				a++;
			}
			else if (jsonC == '}')
			{
				a++;
			}
			else;
		}
		if (a == 2)
		{
			Serial.println(jsonS);
			break;
		}
	}
}
//------**********Uart---------

//--------json字串讀取---------
void ForJson()
{
	//
	jsonC = ' ';
	jsonS = "";
	//
	jsonNumber = 0;
	int a = 0;
	choose = "";
	P[0] = 0;
	P[1] = 0;
	P[2] = 0;
	suck = 0;
	while (1)
	{		
		/*if (My_Serial.available())
		{
			json[jsonNumber] = My_Serial.read();
			if (json[jsonNumber] == '{')
			{
				a++;
			}
			else if (json[jsonNumber] == '}')
			{
				a++;
			}
			else;
			jsonNumber++;
		}*/		

		if (a == 2)
		{
			My_Serial.println(json);
			StaticJsonBuffer<400> jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);
			choose = root["choose"];
			P[0] = root["X"];
			P[1] = root["Y"];
			P[2] = root["Z"];
			suck = root["suck"];
			if (debug)
			{
				My_Serial.println(choose);
				My_Serial.println(P[0]);
				My_Serial.println(P[1]);
				My_Serial.println(P[2]);
				My_Serial.println(suck);
			}
			else;
			a = 0;
			break;
		}
		else;
	}
}
//--------********json字串讀取---------

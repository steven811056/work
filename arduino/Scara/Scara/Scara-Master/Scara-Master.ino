#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Stop.h>
#include "math.h"
#include <Wire.h>
#include <arduino.h>
//#include "MifesSCB.h"

#define first_arm 10
#define second_arm 10

//  slave 1 是控制AB節臂的馬達
//  slave 2 是控制BC節臂的馬達
//  slave 3 是控制Z軸的馬達
const int stepPin[3] = { 2, 4, 6 };
const int dirPin[3] = { 3, 5, 7 };

//int16_t Address[100][3];  //輸入位址的紀錄存放
int16_t AddressGap[3];  //輸入與前一次所在的距離差

int8_t Max = 0;  //運轉圈數

double P[3] = { 0, 0, 0 };  //本次輸入的位置
uint16_t t;  //運轉時間
char choose = '!';  //選擇輸入模式
double L_AB;  //A到B點的距離
double F_CD; //CD直線的斜率
double L_EF, L_AP, L_BP;
double L_AG, L_CG, L_AE, L_BF;
double C_X1, C_X2, C_Y1, C_Y2;
double thetalOne, thetalTwo;  //角BAC 與 直線AC與X的夾角
double thetalThree, thetalFour, thetalTF;
double thetal_A[2] = {0,0};  //A所要轉的方向跟角度
double thetal_A2[2] = {0,0};
double thetal_A3[2] = { 0,0 };
double thetal_B[2] = { 0,0 }; //B所要轉的方向跟角度
double thetal_B2[2] = {0,0};
double thetal_B3[2] = { 0,0 };
double thetal_Z[2] = { 0,0 };
double thetal_Z2[2] = { 0,0 };
double thetal_Z3[2] = { 0,0 };
double turn;  //y比x的比值		

union unionType
{
	byte a[4] = { 0,0,0,0 };
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
	Serial.begin(9600);
	for (int i = 0; i < 3; i++)
	{
		pinMode(stepPin[i], OUTPUT);
		pinMode(dirPin[i], OUTPUT);
	}
	Serial.print("目前初始位置:(");
	for (int i = 0; i < 3; i++)
	{
		Serial.print(P[i]);
		if (i < 2)
			Serial.print(",");
	}
	Serial.println(")");
	Serial.println();
}

void loop()
{
	if (choose == '!')
	{
		Serial.println("選擇模式");
		choose = 0;
	}
	while (1)
	{
		if (Serial.available())
		{
			choose = Serial.read();
			Serial.println(choose);			
			if (choose == '0')
			{
				scara_reset();
				
				break;
			}
			if (choose == '1') //1號模式 輸入座標
			{
				Serial.println("座標模式");
				ctrl_deg();
				choose = '!';
				Serial.println();
				return;
			}			
			if (choose == '2')  //2號模式 輸入角度
			{
				Serial.println("角度模式");
				Serial.println("開始與slave通訊");
				connect();
				Serial.println("結束與slave通訊");
				choose = '!';
				Serial.println();
				break;
			}			
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
	//Emergency_Stop.EMStop();
	Serial.println("輸入座標");
	while (1)
	{		
		if (Serial.available())
		{
			P[0] = Serial.parseInt();
			P[1] = Serial.parseInt();
			P[2] = Serial.parseInt();			
			break;
		}
	}
	Serial.println("座標輸入完成--開始運算");
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (L_AB>20 || L_AB<11)
	{
		Serial.println("輸入錯誤，重新輸入");
		ctrl_deg();
	}
	if (L_AB<21 && L_AB>10)
	{
		Quadrant_Judge();
		delta_3axis();
		choose = '!';
	}
	Serial.println();
}
//********----------------座標輸入---------------

//----------象限判斷-----------
void Quadrant_Judge()
{
	if (P[1]>0)
	{
		if (P[0]>0)
		{
			Serial.println("第一象限");
		}
		if (P[0]<0)
		{
			Serial.println("第二象限");
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
	for (int i = 0; i < 2; i++)
	{
		thetal_A2[i] = thetal_A[i];
		thetal_B2[i] = thetal_B[i];
		thetal_Z2[i] = thetal_Z[i];
	}
	Serial.print("運轉圈數 --> ");
	Serial.println(Max);
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	Serial.print("L_AB -> ");
	Serial.println(L_AB);
	L_CG = L_AG - first_arm;
	L_AE = L_AB - second_arm;
	L_BF = L_AB - first_arm;
	L_EF = L_AB - L_AE - L_BF;
	L_AP = (L_EF / 2) + L_AE;
	L_BP = (L_EF / 2) + L_BF;
	turn = abs(P[1] / P[0]);
	Serial.println(cos(abs(P[1]) / L_AB));
	Serial.print("直線方程式 -> ");
	Serial.print("y = ");	Serial.print(double(P[1]) / double(P[0]));	Serial.println("X");
	Serial.print("垂直於方程式的直線 -> ");
	Serial.print("y = ");	Serial.print((double(P[0]) / double(P[1]))*(-1));	Serial.println("X");
	F_CD = (double(P[0]) / double(P[1]))*(-1);
	Serial.print("F_CD 斜率 -> ");
	Serial.println(F_CD);
	//---
	Serial.print("X   --> ");
	Serial.print(C_X1);
	Serial.print("\t");
	Serial.println(C_X2);
	//--
	thetalOne = acos(L_AP / first_arm) * 180 / PI;
	Serial.print("thetalOne  turn -->");
	Serial.println(cos(L_AP / first_arm));
	Serial.print("thetalOne  turn -->");
	Serial.println(thetalOne);
	//---
	thetalTwo = atan(turn) * 180 / PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
	Serial.print("thetalTwo  turn -->");
	Serial.println(cos(abs(P[1]) / L_AB));
	Serial.print("thetalTwo  turn -->");
	Serial.println(thetalTwo);
	//---
	thetalThree = asin(L_AP / first_arm) * 180 / PI;
	Serial.print("thetalThree  turn -->");
	Serial.println(thetalThree);
	//--
	thetalFour = asin(L_BP / second_arm) * 180 / PI;
	Serial.print("thetalFour  turn -->");
	Serial.println(thetalFour);
	thetalTF = 180 - thetalThree - thetalFour;
	//--	
	Wire.beginTransmission(0x01);
	Wire.write("start");
	Wire.endTransmission();
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
	Wire.beginTransmission(0x03);
	Wire.write("start");
	Wire.endTransmission();
	//--
	thetal_A[1] = thetalOne + thetalTwo;
	if (P[0] < 0)
	{
		thetal_A[1] = 180 - thetalTwo - thetalOne;
	}	
	Serial.print("thetal_A轉動");
	Serial.print(thetal_A[1]);
	Serial.println("度");
	//----
	thetal_Z[1] = P[2];
	thetal_B[1] = thetalTF;
	thetal_A3[1] = thetal_A[1] - thetal_A2[1];
	thetal_B3[1] = thetal_B[1] - thetal_B2[1];
	thetal_Z3[1] = thetal_Z[1] - thetal_Z2[1];
	delay(1);
	if (thetal_A3[1] < 0)
	{
		thetal_A3[0] = 1;
		thetal_A3[1] = -(thetal_A3[1]);
		Serial.println("<0");
	}
	else
	{
		thetal_A3[0] = 0;		
		Serial.println(">0");
	}
	//
	if (thetal_B3[1] < 0)
	{
		thetal_B3[0] = 1;
		thetal_B3[1] = -(thetal_B3[1]);
		Serial.println("<0");
	}
	else
	{
		thetal_B3[0] = 0;	
		Serial.println(">0");
	}
	//
	if (thetal_Z3[1] < 0)
	{
		thetal_Z3[0] = 1;
		thetal_Z3[1] = -(thetal_Z3[1]);
		Serial.println("<0");
	}
	else
	{
		thetal_Z3[0] = 0;	
		Serial.println(">0");
	}
	Serial.print("輸出的3個角度 --> ");
	Serial.print(thetal_A3[0]);
	Serial.print(" <---> ");
	Serial.print(thetal_A3[1]);
	Serial.println();
	Serial.print(thetal_B3[0]);
	Serial.print(" <---> ");
	Serial.print(thetal_B3[1]);
	Serial.println();
	Serial.print(thetal_Z3[0]);
	Serial.print(" <---> ");
	Serial.print(thetal_Z3[1]);
	Serial.println();
	//-----****
	thetal_Z[1] = P[2];
	thetal_B[1] = thetalTF;
	Slave1.b = thetal_A[1];
	Slave2.b = thetal_B[1];
	Slave3.b = thetal_Z[1];	
	Wire.beginTransmission(0x01);
	Serial.println("beginTransmission(0x01)");
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave1.a[i]);
		Serial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x02);
	Serial.println("beginTransmission(0x02)");
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave2.a[i]);
		Serial.println(Slave2.a[i]);
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x03);
	Serial.println("beginTransmission(0x03)");
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave3.a[i]);
		Serial.println(Slave3.a[i]);
	}
	Wire.endTransmission();
	Serial.print("thetal_A--byte -> ");
	Serial.println((byte)thetal_A[1]);
	Serial.print("thetal_B轉動 -> ");
	Serial.print(thetal_B[1]);
	Serial.println("度");
	Serial.print("Z軸旋轉 -> ");	
	Serial.println(P[2]);

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
	Serial.println();
	Serial.println("輸入三軸角度");
	while (1)
	{
		if (Serial.available())
		{
			P[0] = Serial.parseInt();
			P[1] = Serial.parseInt();
			P[2] = Serial.parseInt();
			break;
		}
	}
	Slave1.b = P[0];
	Slave2.b = P[1];
	Slave3.b = P[2];
	Serial.println(Slave1.b);
	Serial.println(Slave2.b);
	Serial.println(Slave3.b);
	Serial.println();
	Wire.beginTransmission(0x01);
	Serial.println("beginTransmission(0x01)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] != 0)
		{
			Wire.write(Slave1.a[i]);
			Serial.println(Slave1.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x02);
	Serial.println("beginTransmission(0x02)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] != 0)
		{
			Wire.write(Slave2.a[i]);
			Serial.println(Slave2.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x03);
	Serial.println("beginTransmission(0x03)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave3.a[i] != 0)
		{
			Wire.write(Slave3.a[i]);
			Serial.println(Slave3.a[i]);
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
	Serial.println("reset");
	Wire.beginTransmission(0x01);
	Wire.write("reset");	
	Wire.endTransmission();		
	Wire.beginTransmission(0x02);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(0x03);
	Wire.write("reset");
	Wire.endTransmission();
	Serial.println("reset END");
	choose = '!';
}
//***********-----------歸零------------



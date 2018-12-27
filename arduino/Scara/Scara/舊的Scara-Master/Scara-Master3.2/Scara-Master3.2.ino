#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
//#include <Stop.h>
#include <math.h>
#include <Wire.h>
#include <arduino.h>
//#include "MifesSCB.h"

#define Microstep_control 0.1	//微步控制倍率	16
//#define Microstep_control 	//微步控制倍率	8
//#define Microstep_control 	//微步控制倍率	4

#define MySerial Serial
#define MySerial_2 Serial
#define MySlave1 0x01
#define MySlave2 0x02
#define MySlave3 0x03
#define MySlave4 0x04

#define first_arm 10	//第一節臂參數
#define second_arm 10	//第二節臂參數
#define Sprint(a) MySerial.print(a)
#define debug 1

//  slave 1 是控制AB節臂的馬達   slave 2 是控制BC節臂的馬達  slave 3 是控制Z軸的馬達
//---josn
char json[150];
int jsonNumber = 0;
double P[3] = { 0, 0, 0 };  //本次輸入的位置
char* choose;	//選擇輸入模式
int suck;		//吸盤
int a;
//---
//---比對
int16_t AddressGap[3];  //輸入與前一次所在的距離差
double P2[3] = { 0,0,0 };	//上次的位置
//----
//----
int8_t Max = 0;  //運轉圈數
uint16_t t;  //運轉時間
int8_t slave_response[4] = { 0,0,0,0 };
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
double thetal_B4 = 0;
double thetal_B_C = 0;	//存放這次減掉上次的值 拿來比較用compare
double thetal_Z[2] = { 0,0 };	 //Z所要轉的方向跟角度
double thetal_Z2[2] = { 0,0 };	//Z上一次所轉的方向跟角度
double thetal_Z3[2] = { 0,0 };	//存放這次減掉上次的值
double turn;  //y比x的比值	
int suck_thetal[2]{ 0, 0 };	//吸盤轉動角度存放位置
//-----

union unionType
{
	byte a[8] = { 0,0,0,0,0,0,0,0 };
	int32_t b;
};

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
			//wait_for_slave();
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
			else {};
			if (suck == 0)
			{
				Wire.beginTransmission(MySlave4);
				Wire.write("close");
				Wire.endTransmission();
			}
			else if (suck == 1)
			{
				Wire.beginTransmission(MySlave4);
				Wire.write("open");
				Wire.endTransmission();
			}
			else if (suck == 2)
			{
				Slave4.b = (suck_thetal[1] / 0.05625);
				Wire.beginTransmission(MySlave4);
				Wire.write("turn");
				Wire.endTransmission();
				delay(100);
				if (suck_thetal[0] == 0)
				{
					MySerial.println(" 正轉 ");
					Wire.beginTransmission(MySlave4);
					Wire.write("start");
					Wire.endTransmission();
					delay(10);
					Wire.beginTransmission(MySlave4);
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
				else if (suck_thetal[0] == 1)
				{
					MySerial.println(" 反轉 ");
					Wire.beginTransmission(MySlave4);
					Wire.write("start2");
					Wire.endTransmission();
					delay(10);
					Wire.beginTransmission(MySlave4);
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
				while (slave_response[3] != 2)
				{
					wait_for_slave();
					delay(50);
				}
				if (debug)
				{
					MySerial.println("吸盤動作完成");
				}
			}
			else if (suck == 3)
			{
				Slave4.b = (suck_thetal[1] / 0.05625);
				Wire.beginTransmission(MySlave4);
				Wire.write("turn");
				Wire.endTransmission();
				delay(100);
				if (suck_thetal[0] == 0)
				{
					MySerial.println(" 正轉 ");
					Wire.beginTransmission(MySlave4);
					Wire.write("start2");
					Wire.endTransmission();
					delay(10);
					Wire.beginTransmission(MySlave4);
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
				else if (suck_thetal[0] == 1)
				{
					MySerial.println(" 反轉 ");
					Wire.beginTransmission(MySlave4);
					Wire.write("start");
					Wire.endTransmission();
					delay(10);
					Wire.beginTransmission(MySlave4);
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
		else if (strcmp(choose, "4") == 0)
		{
			if (debug)
			{
				MySerial.println("檢查通訊");
			}
			wait_for_slave();
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
	if (L_AB>20 || L_AB<11 || L_AB == 0)
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
		//	<<<	座標角度轉換
		Scara_3axis();
		//	>>>	座標角度轉換
		choose = "!";
	}
	if (debug)
	{
		MySerial.println();
	}
}
//********----------------座標輸入---------------

void Scara_3axis()	//----------座標角度轉換------------
{
	Max++;
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
	L_AB = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (debug)
	{
		MySerial.print("L_AB -> ");
		MySerial.println(L_AB);
	}
	L_CG = L_AG - first_arm;
	L_AE = L_AB - second_arm;
	L_BF = L_AB - first_arm;
	L_EF = L_AB - L_AE - L_BF;
	L_AP = (L_EF / 2) + L_AE;
	L_BP = (L_EF / 2) + L_BF;
	turn = abs(P[1] / P[0]);
	/*if (debug)
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
	}*/
	thetalOne = acos(L_AP / first_arm) * 180 / PI;
	//---
	thetalTwo = atan(turn) * 180 / PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
					     //---
	thetalThree = asin(L_AP / first_arm) * 180 / PI;
	//--
	thetalFour = asin(L_BP / second_arm) * 180 / PI;
	thetalTF = 180 - thetalThree - thetalFour;
	thetal_A[1] = thetalOne + thetalTwo;
	if (P[0] < 0)
	{
		thetal_A[1] = 180 - thetalTwo - thetalOne;
	}
	if (debug)
	{
		MySerial.print("thetalOne  turn -->");
		MySerial.println(cos(L_AP / first_arm));
		MySerial.print("thetalOne  turn -->");
		MySerial.println(thetalOne);

		MySerial.print("thetalTwo  turn -->");
		MySerial.println(cos(abs(P[1]) / L_AB));
		MySerial.print("thetalTwo  turn -->");
		MySerial.println(thetalTwo);

		MySerial.print("thetalThree  turn -->");
		MySerial.println(thetalThree);

		MySerial.print("thetalFour  turn -->");
		MySerial.println(thetalFour);

		MySerial.print("thetal_A轉動");
		MySerial.print(thetal_A[1]);
		MySerial.println("度");
	}
	//----
	thetal_Z[1] = P[2];
	thetal_B[1] = thetalTF;
	thetal_A3[1] = thetal_A[1] - thetal_A2[1];
	thetal_B4 = thetal_B[1] - thetal_B2[1];
	//要是這次的B跟上次相減不等於0 就讓第2節臂轉動thetal_B度
	if (thetal_B4 == 0)
	{
		thetal_B3[1] = 0;
	}
	else
	{
		thetal_B3[1] = thetal_B[1];
	}
	//
	thetal_Z3[1] = thetal_Z[1] - thetal_Z2[1];
	//---->>>>>判斷下次是正轉或反轉
	clockwise_counterclockwise();
	//----<<<<<******判斷下次是正轉或反轉		
	if (debug)
	{
		MySerial.println("輸出的3個角度 --> ");
		MySerial.print(thetal_A3[0]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_A3[1]);
		MySerial.println();
		MySerial.print(thetal_B3[0]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_B3[1]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_B4);
		MySerial.println();
		MySerial.print(thetal_Z3[0]);
		MySerial.print(" <---> ");
		MySerial.print(thetal_Z3[1]);
		MySerial.println();
	}
	// <<<	給子版開始訊號
	Wire_start();
	// >>>	給子版開始訊號
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	//	>>>	設定步數的倍率
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
	//	<<<	設定步數的倍率
	//	>>>	吸盤開啟	
	suck_function();
	//	<<<	吸盤開啟
	if (debug)
	{
		MySerial.println("beginTransmission(MySlave3)");
	}
	Wire.beginTransmission(MySlave3);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave3.a[i]);
		MySerial.println(Slave3.a[i]);
	}
	Wire.endTransmission();
	delay(200);
	if (debug)
	{
		MySerial.println("beginTransmission(MySlave1)");
	}
	Wire.beginTransmission(MySlave1);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave1.a[i]);
		MySerial.println(Slave1.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		MySerial.println("beginTransmission(MySlave2)");
	}
	Wire.beginTransmission(MySlave2);
	for (int i = 0; i < 4; i++)
	{
		Wire.write(Slave2.a[i]);
		MySerial.println(Slave2.a[i]);
	}
	Wire.endTransmission();
	if (debug)
	{
		MySerial.print("thetal_A--byte -> ");
		MySerial.println((byte)thetal_A[1]);
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
	//wait_for_slave();
}
//	<<<	座標角度轉換------------

void suck_function()	//	>>>	吸盤演算法-------
{
	if (thetal_A3[0] == thetal_B3[0])
	{
		suck_thetal[0] = !thetal_A3[0];
		suck_thetal[1] = thetal_A3[1] + thetal_B3[1];
	}
	else if (thetal_A3[0] != thetal_B3[0])
	{
		if (thetal_A3[1] > thetal_B3[1])
		{
			suck_thetal[0] = thetal_B3[0];
			suck_thetal[1] = fabs(thetal_A3[1] - thetal_B3[1]);
		}
		if (thetal_A3[1] < thetal_B3[1])
		{
			suck_thetal[0] = thetal_A3[0];
			suck_thetal[1] = fabs(thetal_A3[1] - thetal_B3[1]);
		}
	}
}
//	<<<	吸盤演算法-------

void Wire_start()		// <<<	給子版開始訊號
{
	if (thetal_A3[0] == 0)
	{
		if (debug)
		{
			MySerial.print("第1節臂 正轉 ");
			MySerial.println(" = 0");
		}
		Wire.beginTransmission(MySlave1);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			MySerial.print("第1節臂 反轉 ");
			MySerial.println(" != 0");
		}
		Wire.beginTransmission(MySlave1);
		Wire.write("start2");
		Wire.endTransmission();
	}
	if (thetal_B3[0] == 0)
	{
		if (debug)
		{
			MySerial.print("第2節臂 正轉 ");
			MySerial.println(" = 0");
		}
		Wire.beginTransmission(MySlave2);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			MySerial.print("第2節臂 反轉 ");
			MySerial.println(" != 0");
		}
		Wire.beginTransmission(MySlave2);
		Wire.write("start2");
		Wire.endTransmission();
	}
	if (thetal_Z3[0] == 0)
	{
		if (debug)
		{
			MySerial.print("Z 正轉 ");
			MySerial.println(" = 0");
		}
		Wire.beginTransmission(MySlave3);
		Wire.write("start");
		Wire.endTransmission();
	}
	else
	{
		if (debug)
		{
			MySerial.print("Z 反轉 ");
			MySerial.println(" != 0");
		}
		Wire.beginTransmission(MySlave3);
		Wire.write("start2");
		Wire.endTransmission();
	}
}
//	>>>	給子版開始訊號

//	 <<<	判斷正轉或反轉
void clockwise_counterclockwise()
{
	if (thetal_A3[1] < 0)
	{
		thetal_A3[0] = 1;
		thetal_A3[1] = -(thetal_A3[1]);
	}
	else
	{
		thetal_A3[0] = 0;
	}
	if (thetal_Z3[1] < 0)
	{
		thetal_Z3[0] = 1;
		thetal_Z3[1] = -(thetal_Z3[1]);
	}
	else
	{
		thetal_Z3[0] = 0;
	}
	//>>>>>>新的第2節臂轉動演算法
	//--	>>	原 第2臂判斷正反轉的地方	
	if (P[0] >= 0)
	{
		thetal_B[0] = 1;
	}
	if (P[0] < 0)
	{
		thetal_B[0] = 0;
	}
	//--	<<	原 第2臂判斷正反轉的地方	
	if (thetal_B[0] != thetal_B2[0])
	{
		thetal_B3[0] = thetal_B[0];
		thetal_B3[1] = thetal_B[1] + thetal_B2[1];
	}
	else if (thetal_B[0] == thetal_B2[0])
	{
		thetal_B3[1] = thetal_B[1] - thetal_B2[1];
		if (thetal_B3[1] > 0)
		{
			thetal_B3[0] = thetal_B[0];
		}
		else if (thetal_B3[1] < 0)
		{	//讓thetal_B3 成為 thetal_B方向的not閘
			thetal_B3[0] = !thetal_B[0];
			thetal_B3[1] = (-1.0)*(thetal_B3[1]);
		}
		else if (thetal_B3[1] == 0)
		{
			thetal_B3[0] = 0;
			thetal_B3[1] = 0;
		}
	}
	//<<<<<-**********新的第2節臂轉動演算法
}
// >>>  判斷正轉或反轉

//-----等待子版回復-----
void wait_for_slave()
{	/*	 slave_response[i] 各板子的狀態 0沒上線 1上線	*/
	for (int i = 0; i < 4; i++)
	{
		slave_response[i] = 0;
	}
	if (debug)
	{
		MySerial.println("*->In wait_function");
	}
	for (int i = 0; i < 4; i++)
	{
		Wire.requestFrom((MySlave1 + i), 1);
		if (Wire.available())
		{
			slave_response[i] = Wire.read();
		}
	}
	if (strcmp(choose, "4") == 0)
	{
		if (debug)
		{
			for (int i = 0; i < 4; i++)
			{
				if (slave_response[i] >= 1 && slave_response[i] <= 15)
				{
					MySerial.print((MySlave1 + i));
					MySerial.println(" -> ON-line");
				}
				else if (slave_response[i] == 0)
				{
					MySerial.print((MySlave1 + i));
					MySerial.println(" -> off-line");
				}
			}
		}
	}
}
//-----********等待子版回復-----

//---------度數直接控制-----------
void connect()
{
	unionType Slave1;
	unionType Slave2;
	unionType Slave3;
	Wire.beginTransmission(MySlave1);
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(MySlave2);
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(MySlave3);
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
	Wire.beginTransmission(MySlave1);
	MySerial.println("beginTransmission(MySlave1)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave1.a[i] != 0)
		{
			Wire.write(Slave1.a[i]);
			MySerial.println(Slave1.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(MySlave2);
	MySerial.println("beginTransmission(MySlave2)");
	for (int i = 0; i < 4; i++)
	{
		if (Slave2.a[i] != 0)
		{
			Wire.write(Slave2.a[i]);
			MySerial.println(Slave2.a[i]);
		}
	}
	Wire.endTransmission();
	Wire.beginTransmission(MySlave3);
	MySerial.println("beginTransmission(MySlave3)");
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
{ /*  歸零方式我定為AB臂轉180度壓住極限開關
  ，BC臂為壓到極限開關後往反方向旋轉135度  */
	if (debug)
	{
		MySerial.println("reset");
	}
	Wire.beginTransmission(MySlave3);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(MySlave1);
	Wire.write("reset");
	Wire.endTransmission();
	Wire.beginTransmission(MySlave2);
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
		thetal_A[i] = 0;		thetal_A2[i] = 0;		thetal_A3[i] = 0;
		thetal_B[i] = 0;		thetal_B2[i] = 0;		thetal_B3[i] = 0;
		thetal_B4 = 0;
		thetal_Z[i] = 0;		thetal_Z2[i] = 0;		thetal_Z3[i] = 0;
	}
	suck = 0;	//吸盤
	turn = 0;
	Max = 0;
	choose = "!";
}
//***********-----------歸零------------

//--------json字串讀取---------
void ForJson()
{
json_start:		
	choose = "";
	P[0] = 0;
	P[1] = 0;
	P[2] = 0;
	suck = 0;
	int size_json = sizeof(json) / sizeof(json[0]);
	for (int i = 0; i < size_json; i++)
	{
		json[i] = 0;
	}
	json_reset();
	while (1)
	{
		if (MySerial_2.available())
		{			
			while (MySerial_2.available() > 0)
			{
				json[jsonNumber] = MySerial_2.read();
				//MySerial.println(json);
				if (json[jsonNumber] == '{')
				{					
					goto out1;
				}					
			}				
		}					
	}	
out1:
	jsonNumber++;
	while (1)
	{		
		if (MySerial_2.available())
		{
			while (MySerial_2.available() > 0)
			{
				json[jsonNumber] = MySerial_2.read();
				//MySerial.println(json);				
				if (json[jsonNumber] == '}')
				{
					goto out2;
				}
				jsonNumber++;
			}
		}
	}
out2:		
	MySerial.println(json);
	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);
	if (!root.success())
		goto json_start;
	choose = root["choose"];
	P[0] = root["X"];
	P[1] = root["Y"];
	P[2] = root["Z"];
	suck = root["suck"];
	if (debug)
	{
		MySerial.println(choose);
		MySerial.println(P[0]);
		MySerial.println(P[1]);
		MySerial.println(P[2]);
		MySerial.println(suck);
	}
	while (MySerial_2.available() > 0)
	{
		MySerial_2.read();		
	}
}
void json_reset()
{
	a = 0;
	jsonNumber = 0;
}
//--------********json字串讀取---------

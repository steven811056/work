#include "math.h"
#include <Wire.h>
#include <EEPROM.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

#include "Scara_3Axis.h"

// W5100的初始設定
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,50,200 };
byte server[] = { 192,168,50,156 }; 
EthernetClient client;
StaticJsonBuffer<600> jsonBuffer;

#define first_arm 10	//第一節臂長度 cm
#define second_arm 10	//第二節臂長度
#define slave1 1	//  slave 1 是控制AB節臂的馬達
#define slave2 2	//  slave 2 是控制BC節臂的馬達
#define slave3 3	//  slave 3 是控制Z軸的馬達

//int16_t Address[100][3];  //輸入位址的紀錄存放
//int16_t AddressGap[3];  //輸入與前一次所在的距離差
int16_t a = 0;	//跳脫while迴圈用的變數
int16_t Max = 0;  //運轉圈數
int16_t P[2][3] = { {0,0,0},{0,0,0} };  //本次、前一次輸入的存放位置
uint16_t Running;  //運轉時間
int16_t Test1 = 0, Test2 = 0, Test3 = 0;
char choose = '!';  //選擇輸入模式
double L_AC;  //A到C點的距離
double thetalOne;   //角BAC
double thetalTwo;	//直線AC與X的夾角
double thetal_A[2];  //第一節臂所要轉的方向跟角度
double thetal_B[2];  //第二節臂所要轉的方向跟角度
double turn[3];  //三軸所要運轉的角度
//char Hi;   //我所使用的歸零代號
//int16_t zero=10; //歸零用的感測器
int16_t delaytime = 30;

const int stepPin[3] = { 2, 4, 6 };
const int dirPin[3] = { 3, 5, 7 };

void setup()
{
		Wire.begin();
		Serial.begin(9600);	
		Serial.print("目前初始位置:(");
		for (int i = 0; i < 3; i++)
		{
				Serial.print(P[0][i]);
				if (i < 2)
					Serial.print(",");
		}
		Serial.println(")");
		Serial.println();

		for (int i = 0; i < 3; i++)
		{
				pinMode(stepPin[i], OUTPUT);
				pinMode(dirPin[i], OUTPUT);
		}
}

void loop()
{
	while (choose == '!')
	 {
			while (a == 0)
			 {
					Serial.println("選擇模式");
					Serial.println("0 ->歸零  1 ->點控制  2->角度控制");
					a++;
			 }		
			if (Serial.available())
			{
					choose = Serial.read();
					a = 0;
			}		
	 }
	while (choose == '0')
	{
			Serial.println("進入歸零");
			scara_reset();
	}
	if (choose == '1')
	{
			Serial.println("進入X , Y , Z模式");
			Serial.println("請輸入 X , Y , Z");
			a++;
			while (a > 0)
			 {
					if (Serial.available())
					{
							P[0][0] = Serial.parseInt();
							P[0][1] = Serial.parseInt();
							P[0][2] = Serial.parseInt();
							a = 0;
					}
					for (int i = 0; i < 3; i++)
					{
							P[1][i] = P[0][i];
					}
			
			 }
			delta_3axis();
			Serial.println(L_AC);
			Serial.println(acos((L_AC / 2) / first_arm));
			Serial.println(thetalOne);
			Serial.println((P[0][1]) / (P[0][0]));
			Serial.println(thetalTwo);
			//delta_3axis_movie();
	}
	while (choose == '2')
	 {
			Serial.println("進入角度控制模式");

	 }
	
}

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

void delta_3axis()
{
		
		L_AC = sqrt(pow(P[0][0], 2) + pow(P[0][1], 2));   
		thetalOne = acos((L_AC/2)/first_arm) * 180 / PI; 	//arduino的三角函數出來都是弧度，需要*180/PI	
		thetalTwo = atan((double)(P[0][1]) /( P[0][0])) * 180 / PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
		thetal_B[1] = (180 - (2 * thetalOne));
		thetal_A[1] = thetalOne + thetalTwo;		
		Wire.beginTransmission(slave1);
		Serial.println("Trans with slave1");
		Wire.write("turn");
		Wire.endTransmission();
		Serial.println("end Trans with slave1");
		/*Wire.beginTransmission(slave1);
		Wire.write((byte)thetal_A[1]);		
		Wire.endTransmission();*/
		Serial.print("thetal_A轉動");
		Serial.print(thetal_A[1]);
		Serial.println("度");
		Serial.print("thetal_A--byte->");
		Serial.println((byte)thetal_A[1]);
		Serial.print("thetal_B轉動");
		Serial.print(thetal_B[1]);
		Serial.println("度");
}

void delta_3axis_movie()
{
		digitalWrite(dirPin[0], HIGH);
		delay(30);
		for (int i = 0; i < ((thetalOne/0.1125)*20); i++)
		{
			digitalWrite(stepPin[0], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[0], LOW);
			delayMicroseconds(delaytime);
		}
		delay(50);

		digitalWrite(dirPin[2], LOW);
		delay(10);
		for (int i = 0; i < (P[0][2] * 3200); i++)
		{
			digitalWrite(stepPin[2], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[2], LOW);
			delayMicroseconds(delaytime);
		}
		delay(10);

		digitalWrite(dirPin[1], HIGH);
		delay(10);
		for (int i = 0; i <  ((thetalTwo / 0.1125) * 20); i++)
		{
			digitalWrite(stepPin[1], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[1], LOW);
			delayMicroseconds(delaytime);
		}
		delay(10);
		delay(2000);

		digitalWrite(dirPin[0], LOW);
		delay(30);
		for (int i = 0; i < ((thetalOne / 0.1125) * 20); i++)
		{
			digitalWrite(stepPin[0], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[0], LOW);
			delayMicroseconds(delaytime);
		}
		delay(50);

		digitalWrite(dirPin[2], HIGH);
		delay(10);
		for (int i = 0; i < (P[0][2] * 3200); i++)
		{
			digitalWrite(stepPin[2], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[2], LOW);
			delayMicroseconds(delaytime);
		}
		delay(10);

		digitalWrite(dirPin[1], LOW);
		delay(10);
		for (int i = 0; i < ((thetalTwo / 0.1125) * 20); i++)
		{
			digitalWrite(stepPin[1], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[1], LOW);
			delayMicroseconds(delaytime);
		}
		delay(10);
}

void scara_reset()
{ /*  歸零方式我定為AB臂轉180度壓住極限開關
  ，BC臂為壓到極限開關後往反方向旋轉135度  */
		for (int i = 0; i<2; i++)
		{
				thetalOne = 0;
				thetalTwo = 0;
				thetal_A[i] = 0;
				thetal_B[i] = 0;
		}
		Wire.beginTransmission(1);
		Wire.write(3);
		Wire.write("123");
		Wire.endTransmission();
		Wire.beginTransmission(1);
		//  Wire.write(33);  //ascii碼傳輸!  之前用3沒有顯是是因為ascii碼的3是^C的意思
		Wire.write(0);
		Wire.write(3);
		Wire.endTransmission();
		Serial.println("reset");
		Wire.beginTransmission(2);
		Wire.write(3);
		Wire.endTransmission();
}




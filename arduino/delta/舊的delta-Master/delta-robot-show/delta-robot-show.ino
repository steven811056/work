#include "DeltaRobInverseKin.h"
#include "math.h"
#include "My.h"
#include <Wire.h>

#define L_UPPER 0.12 //上臂長    單位：公尺
#define L_LOWER 0.285 //下臂長
#define WB 0.13  //底座正三角型重心到邊的長度
#define WP 0.013  //下方小正三角形重心到邊長的長度
#define UP 0.026  //下方小正三角形重心到頂點的長度
#define SP 0.045  //下方正三角型的邊長

boolean debug = 1;

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);

double coordinate[3];     //輸入的x , y , z
int16_t D[3] = { 0,0,0 }; //抓取演算法出來的goalPos的角度
uint8_t returnData = 0; //歸零完傳回的數值
double degreeTurn[1][3];    //與前一次相減後運轉方向與角度
double location[2][3];  //用來存之前的位置
int16_t locationNumber = 0;
int16_t ring;   //輸入位置的次數  暫時沒用到
int shownumber;


union unionType
{
	byte a[2];
	int b;
};

void setup()
{
	Serial.begin(9600);
	Wire.begin(); 

	delta.debugFlag = false;  
	pinMode(8, INPUT);
	digitalWrite(8, HIGH);
	shownumber = 0;
	Serial.println(" start ");
	Serial.println();
	for (int i = 0; i < 2; i++)
	{
		for (int i2 = 0; i2< 3; i2++)
		{
			location[i][i2] = 0;
		}
	}

}

void loop()
{ 
	if (digitalRead(8) == 0)
	{
		shownumber = 1;
	} 
	if (shownumber == 1)
	{
		Wire.beginTransmission(0x01);
		Wire.write(1);
		Wire.endTransmission();
		Wire.beginTransmission(0x02);
		Wire.write(1);
		Wire.endTransmission();
		Wire.beginTransmission(0x03);
		Wire.write(1);
		Wire.endTransmission();
		Serial.println("beginTransmission(1);");
		Serial.println("beginTransmission(2);");
		Serial.println("beginTransmission(3);");
		shownumber = 0;
	} 

}

void Show()
{
  unionType Slave1;
  unionType Slave2;
  unionType Slave3;
  Slave1.b = 130;
  Slave2.b = 130;
  Slave3.b = 400;
  if (Serial.available())
  {
    Serial.println("hi");
    shownumber = Serial.parseInt();
  }
  if (shownumber == 1)
  {
    Serial.println("hi   1");
    Wire.beginTransmission(1);
    Wire.write("start");
    Wire.endTransmission();
    Wire.beginTransmission(2);
    Wire.write("start");
    Wire.endTransmission();
    Wire.beginTransmission(0x03);
    Wire.write("start");
    Wire.endTransmission();
    Serial.println("beginTransmission(1);");
    Serial.println("beginTransmission(2);");
    Serial.println("beginTransmission(3);");
    delay(10);
    Wire.beginTransmission(1);
    Wire.write(Slave1.a[0]);
    Wire.endTransmission();
    Wire.beginTransmission(2);
    Wire.write(Slave2.a[0]);
    Wire.endTransmission();
    Wire.beginTransmission(0x03);
    Wire.write(Slave3.a[0]);
    Wire.write(Slave3.a[1]);
    Wire.endTransmission();
    shownumber = 0;
  }
}


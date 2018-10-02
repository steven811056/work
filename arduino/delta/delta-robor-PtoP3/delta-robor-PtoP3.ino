#include "DeltaRobInverseKin.h"
#include "math.h"
#include "My.h"

#define L_UPPER 0.12 //上臂長		單位：公尺
#define L_LOWER 0.285 //下臂長
#define WB 0.13  //底座正三角型重心到邊的長度
#define WP 0.013  //下方小正三角形重心到邊長的長度
#define UP 0.026  //下方小正三角形重心到頂點的長度
#define SP 0.045  //下方正三角型的邊長


const int stepPin[3] = { 2,4,6 };
const int dirPin[3] = { 3,5,7 };
const int senser[3] = { 8,9,10 };
const int delaytime = 3;
//const double delaytime = 20;
double coordinate[3];			//輸入的x , y , z
int16_t D[3] = { 0,0,0 };	//抓取演算法出來的goalPos的角度
uint8_t A = 0;
double degreeTurn[1][3];		//與前一次相減後運轉方向與角度
double location[2][3];	//用來存之前的位置
int16_t locationNumber = 0;
int16_t ring;		//輸入位置的次數  暫時沒用到

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
MyDelta Mdelta(location[0][0], location[0][1], location[0][2]);

void setup()
{
	Serial.begin(9600);
	delta.debugFlag = true;
	for (int i = 0; i < 3; i++)
	{
		pinMode(senser[i], INPUT);
		pinMode(stepPin[i], OUTPUT);
		pinMode(dirPin[i], OUTPUT);

	}
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
	while (1)
	{
		Serial.println();
		Serial.println("Input Targets -> X , Y , Z");
		PtoP_input();
		MyDelta Mdelta(coordinate[0], coordinate[1], coordinate[2]);
		//ctrl_PtoP(coordinate[0], coordinate[1], coordinate[2]);
		Mdelta.PtoP();
		
	
	}
}

//----PtoP-----輸入X Y Z的函式
void PtoP_input()
{
	//Mdelta.LocationCompare();
	Serial.println("PtoP input  \t Start");
	int D = 0;
	while (1)
	{
		if (D == 0)
		{
			Serial.println("INPUT -> X");
			while (1)
			{
				if (Serial.available())
				{
					coordinate[0] = Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 1)
		{
			Serial.println("INPUT -> Y");
			while (1)
			{
				if (Serial.available())
				{
					coordinate[1] = Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 2)
		{
			Serial.println("INPUT -> Z");
			while (1)
			{
				if (Serial.available())
				{
					coordinate[2] = 0 - Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 3)
		{
			Serial.print("目前X , Y , Z 軸參數 = ");
			Serial.print(coordinate[0]);
			Serial.print(" , ");
			Serial.print(coordinate[1]);
			Serial.print(" , ");
			Serial.print(coordinate[2]);
			Serial.println();
			D = 0;
			Serial.println("PtoP input	END ");
			break;
		}

	}
}
//----PtoP-----end----------

//------turn-----
void turn()		//馬達轉動函式
{
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(dirPin[i], HIGH);
		delay(50);
		for (int k = 0; k < degreeTurn[0][1]; k++)
		{
			digitalWrite(stepPin[i], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[i], LOW);
			delayMicroseconds(delaytime);
		}
		delay(50);
	}

}
//------turn-----end--------

//--------PtoP_output----------
void PtoP_output()
{
	Serial.println();
	Serial.println("PtoP_output");
	OK();		
	Serial.print("三顆馬達轉動角度 = ");
	for (int i = 0; i < 3; i++)
	{
		Serial.print(delta._theta2[i]);
		if (i < 2)
		{
			Serial.print(" , ");
		}
	}
	Serial.println();
	Serial.print("三顆馬達轉動角度 2= ");
	for (int i = 0; i < 3; i++)
	{
		Serial.print(D[i]);
		if (i < 2)
		{
			Serial.print(" , ");
		}
	}
	if (delta.maxArrIndex > 1)
	{
		Serial.println();
		Serial.println(delta.maxArrIndex);
		for (int i = 0; i < 3; i++)
		{
			Serial.println(location[delta.maxArrIndex - 2][i]);
		};
		Serial.print("兩次輸入度數差 = ");
		for (int i = 0; i < 3; i++)
		{
			Serial.print(degreeTurn[0][i]);
			if (i < 2)
			{
				Serial.print(" , ");
			}
		}
	}

}
//--------PtoP_output----------end-----

void OK()
{
	Serial.println("OK");
	Serial.println();
}


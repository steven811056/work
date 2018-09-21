#include "DeltaRobInverseKin.h"
#include "math.h"
//#include "My.h"

#define L_UPPER 0.12 //上臂長		單位：公尺
#define L_LOWER 0.285 //下臂長
#define WB 0.13  //底座正三角型重心到邊的長度
#define WP 0.013  //下方小正三角形重心到邊長的長度
#define UP 0.026  //下方小正三角形重心到頂點的長度
#define SP 0.045  //下方正三角型的邊長


const int stepPin[3] = { 2,4,11 };
const int dirPin[3] = { 3,5,12 };
const int senser[3] = { 8,9,10 };
const int delaytime = 40;
//const double delaytime = 20;

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
//MyDelta DeltaTest();

int16_t x; //宣告16位元的整數
int16_t y;
int16_t z;
int16_t D[3] = { 0,0,0 };
int16_t Dt;
float x1 = 0, x2, x3 = 0, v1 = 0, v2;
int16_t Drs;
int16_t Dra;
long MyTime = 0;
long MyTime2 = 0;
uint8_t A = 0;
uint8_t deg = 0;
double MaxSpeed;
double AddSpeed;
double Add = 0;
double add[3];
double degreeTurn[1][3];
int16_t ring;

void setup()
{
	Serial.begin(9600);
	delta.debugFlag = true;
	for (int i = 0; i < 3; i++)
	{
		pinMode(senser[i + 8], INPUT);
		pinMode(stepPin[i], OUTPUT);
		pinMode(dirPin[i], OUTPUT);
	}
	Serial.print("Start:(");
	for (int i = 0; i < 3; i++)
	{
		Serial.print(delta.posArr[0][0]);
		if (i < 2)
		{
			Serial.print(",");
		}
	}
	Serial.println(")");
	Serial.println();
	ring = 0;
}


void loop()
{
	while (1)
	{
		Serial.println();
		Serial.println("Target->X , Y , Z");
		PtoP_input();
		ctrl_PtoP(x, y, z);
	}

}

//----PtoP-----
void PtoP_input()
{
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
					x = Serial.parseInt();
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
					y = Serial.parseInt();
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
					z = Serial.parseInt();
					D++;
					break;
				}
			}
		}
		if (D == 3)
		{
			Serial.print("目前X , Y , Z 軸參數 = ");
			Serial.print(x);
			Serial.print(" , ");
			Serial.print(y);
			Serial.print(" , ");
			Serial.print(z);
			Serial.println();
			D = 0;
			ring++;
			if (x == 0 && y == 0 && z == 0)
			{
				//DeltaTest.return1();
				returnO();

			}
			break;
		}

	}
}

//----PtoP-----end----------

//-------ctrl_PtoP---------
void ctrl_PtoP(int16_t x0, int16_t y0, int16_t z0)
{
	Serial.print("ctrl_PtoP");
	OK();
	Serial.print(x0);
	Serial.print(" , ");
	Serial.print(y0);
	Serial.print(" , ");
	Serial.print(z0);
	Serial.println();
	delta.setGoalCoordinates(x0*0.01, y0*0.01, z0*0.01, 0);
	delay(10);
	PtoP_output();
}
//-------ctrl_PtoP---------end--------

//--------PtoP_output----------
void PtoP_output()
{
	Serial.println();
	Serial.println("PtoP_output");
	OK();
	for (int i = 0; i < 3; i++)
	{
		D[i] = delta.goalPos[i];
		degreeTurn[0][i] = D[i] - delta.posArr[delta.maxArrIndex - 2][i];
	}
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
			Serial.println(delta.posArr[delta.maxArrIndex - 2][i]);
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
	turn();

}
//--------PtoP_output----------end-----

//------turn-----
void turn()
{
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(dirPin[i], HIGH);
		delay(50);
		for (int k = 0; k<degreeTurn[0][1]; k++)
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

//---------reutrnO---------
void returnO()
{
	Serial.println("歸零啟動");
	Serial.println((int)digitalRead(8));
	Serial.println(digitalRead(senser[1]));
	Serial.println(digitalRead(senser[2]));
	while (digitalRead(senser[0]) == HIGH || digitalRead(senser[1]) == HIGH || digitalRead(senser[2]) == HIGH)
	{
		Serial.println("歸零啟動1");
		digitalWrite(dirPin[0], HIGH);
		digitalWrite(dirPin[1], HIGH);
		digitalWrite(dirPin[2], HIGH);
		delay(30);

		if (digitalRead(senser[2]) == HIGH)
		{
			digitalWrite(stepPin[2], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[2], LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[2]  start");

		}
		if (digitalRead(senser[2]) == LOW)
		{
			Serial.println("senser[2]  OK");

		}


		if (digitalRead(senser[1]) == HIGH)
		{
			digitalWrite(stepPin[1], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[1], LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[1]  start");

		}
		if (digitalRead(senser[1]) == LOW)
		{
			Serial.println("senser[1]  OK");

		}



		if (digitalRead(senser[0]) == HIGH)
		{
			digitalWrite(stepPin[0], HIGH);
			delayMicroseconds(delaytime);
			digitalWrite(stepPin[0], LOW);
			delayMicroseconds(delaytime);
			Serial.println("senser[0]  start");

		}
		if (digitalRead(senser[0]) == LOW)
		{
			Serial.println("senser[0]  OK");

		}

	}

}

void OK()
{
	Serial.println("OK");
	Serial.println();
}


#include "DeltaRobInverseKin.h"
#include "math.h"

#define L_UPPER 0.12 //上臂長
#define L_LOWER 0.285 //下臂長
#define WB 0.075  //底座正三角型重心到邊的長度
#define WP 0.02  //下方小正三角形重心到邊長的長度
#define UP 0.04  //下方小正三角形重心到頂點的長度
#define SP 0.07  //下方正三角型的邊長

const int stepPin[3] = { 2,4,6 };
const int dirPin[3] = { 3,5,7 };

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
int16_t x; //宣告16位元的整數
int16_t y;
int16_t z;
int16_t D[3] = {0,0,0};
int16_t Dt;
float x1 = 0, x2, x3 = 0, v1 = 0, v2;
int16_t Drs;
int16_t Dra;
long MyTime = 0;
long MyTime2 = 0;
uint8_t A = 0;
uint8_t deg = 0;
boolean MyDirection[3]; //1為逆時針0為順時針
double MaxSpeed;
double AddSpeed;
double Add = 0;
double add[3];
int degreeTurn[3];

void setup()
{
	Serial.begin(9600);
	delta.debugFlag = true;
	for (int i = 0; i < 3; i++)
	{
		pinMode(stepPin[i], OUTPUT);
		pinMode(dirPin[i], OUTPUT);
	}
	Serial.print("目前初始位置:(");
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
}


void loop()
{
	while (1)
	{
		Serial.println();
		Serial.println("輸入目標點的X , Y , Z");
		PtoP_input();
		ctrl_PtoP(x, y, z);
	}
}

//----PtoP-----
void PtoP_input()
{
	int D=0;
	while (1)
	{
		if (D == 0)
		{
			Serial.println("輸入X軸參數");
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
			Serial.println("輸入Y軸參數");
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
			Serial.println("輸入Z軸參數");
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
			break;
		}		

	}
}

//----PtoP-----end----------

//-------ctrl_PtoP---------
void ctrl_PtoP(int16_t x0, int16_t y0 , int16_t z0)
{
	Serial.println("ctrl_PtoP");
	OK();
	Serial.print(x0);
	Serial.print(" , ");
	Serial.print(y0);
	Serial.print(" , ");
	Serial.print(z0);
	Serial.println();
	delta.setGoalCoordinates(x0*0.001, y0*0.001, z0*0.001,0);
	delay(10);
	PtoP_output();
}
//-------ctrl_PtoP---------end--------

//--------PtoP_output----------
void PtoP_output()
{
	Serial.println("PtoP_output");
	OK();
	for (int i = 0; i < 3; i++)
	{
		D[i] = delta.goalPos[i];
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
	
}

//--------PtoP_output----------end-----

//--------ctrl_deg(int16_t d1, int16_t d2, int16_t d3, uint16_t rs, uint16_t ra)----------
//void ctrl_deg(int16_t d1, int16_t d2, int16_t d3, uint16_t t)
//{
//	delta.posArr[deg][0] = d1;    //把輸入的角度存入posArr跟goalPos陣列之中
//	delta.posArr[deg][1] = d2;    //posArr是前100筆移動的位置
//	delta.posArr[deg][2] = d3;
//	delta.goalPos[0] = d1;
//	delta.goalPos[1] = d2;
//	delta.goalPos[2] = d3;
//	deg++;                    //總圈數
//	DegreeOutput();
//}
//----------------ctrl_deg()  END---------------

//---------DegreeAddSpeed()-------------
// 最大速度為HIGH-delaymicroseconds(800)-LOW-delaymicroseconds(800)
// 每送一個波花0.0016秒 1秒625個波 轉70.3125
//void DegreeAddSpeed()
//{  
//  for(int i=0;i<3;i++)
//  {
//    add[i]=2*delta.goalPos[i]/pow(Dt,2); // a=a*x/t^2
//    if(add[i]>70)
//    {
//      Serial.println("錯誤");
//      ctrl_deg(D1,D2,D3,Dt);
//    }
//  }
//}
//---------DegreeAddSpeed()  END-------------

//void DegreeContralSpeed()
//{
//	for (int A = 0; A<3; A++)
//	{
//		Dra = 2 * delta.mygoalPos[A] / pow(Dt, 2);
//		for (int i = 0; i<Dt; i++)
//		{
//			x2 = 0.5*Dra*pow(i, 2);
//			if (i>0)
//			{
//				x1 = 0.5*Dra*pow((i - 1), 2);
//			}
//			x3 = x2 - x1;
//			if ((x3 - int(x3)) * 10 >= 5)
//			{
//				x3 = x3 + 1;
//			}
//			if (x3>0)
//			{
//				for (int k = 0; k<x3; k++)
//				{
//					digitalWrite(stepPin[i], HIGH);
//					delayMicroseconds(800);
//					digitalWrite(stepPin[i], LOW);
//					delayMicroseconds(800);
//				}
//			}
//		}
//	}
//}

//---------------DegreeOutput()----------------
//void DegreeOutput()
//{
//	if (A>0)
//	{
//		A = A - 1;
//		Serial.print("Start at :(");
//		for (int i = 0; i<3; i++)
//		{
//			Serial.print(delta.posArr[A][i]);
//			if (i<2)
//				Serial.print(",");
//		}
//		Serial.println(")");
//
//		for (int i = 0; i<3; i++)
//		{
//			if (delta.posArr[A + 1][i]>0 && delta.posArr[A][i]>0)
//			{
//				if (delta.posArr[A + 1][i] > delta.posArr[A][i]) // ex:100->150
//				{
//					MyDirection[i] = 0;
//				}
//				else // ex:150->100
//				{
//					MyDirection[i] = 1;
//				}
//				delta.mygoalPos[i] = abs(delta.posArr[A + 1][i]) - abs(delta.posArr[A][i]);
//			}
//			if (delta.posArr[A + 1][i]<0 && delta.posArr[A][i]<0)
//			{
//				if (delta.posArr[A + 1][i] > delta.posArr[A][i])  // ex:200->210
//				{
//					MyDirection[i] = 0;
//				}
//				else  // ex:210->200
//				{
//					MyDirection[i] = 1;
//				}
//				delta.mygoalPos[i] = abs(delta.posArr[A + 1][i]) - abs(delta.posArr[A][i]);
//			}
//			if (delta.posArr[A + 1][i]<0 && delta.posArr[A][i]>0)  // ex:150->200
//			{
//				MyDirection[i] = 0;
//				delta.mygoalPos[i] = 180 - abs(delta.posArr[A + 1][i]) + 180 - abs(delta.posArr[A][i]);
//			}
//			if (delta.posArr[A + 1][i]>0 && delta.posArr[A][i]<0)  //ex:200->150
//			{
//				MyDirection[i] = 1;
//				delta.mygoalPos[i] = 180 - abs(delta.posArr[A + 1][i]) + 180 - abs(delta.posArr[A][i]);
//			}
//
//		}
//		A++;
//	}
//	else
//	{
//		Serial.println("first print");
//	}
//
//	Serial.print("End at ");
//	Serial.print("output (");
//	Serial.print(delta.goalPos[0]);
//	Serial.print(", ");
//	Serial.print(delta.goalPos[1]); //單位為0.01度
//	Serial.print(", ");
//	Serial.print(delta.goalPos[2]);
//	Serial.println(")");
//	Serial.println();
//	if (A>0)
//	{
//		Serial.print("運轉度數差 :(");
//		for (int i = 0; i<3; i++)
//		{
//			Serial.print(delta.mygoalPos[i]);
//			if (i<2)
//				Serial.print(",");
//		}
//		Serial.println(")");
//		Serial.println();
//	}
//	A++;
//}
//---------------DegerrOutput()  END------------------



//void input()
//{
//	while (1)
//	{
//		if (Serial.available())
//		{
//			D1 = Serial.parseInt();
//			D2 = Serial.parseInt();
//			D3 = Serial.parseInt();
//			Dt = Serial.parseInt();
//			Serial.print("input (");
//			Serial.print(D1);
//			Serial.print(", ");
//			Serial.print(D2);
//			Serial.print(", ");
//			Serial.print(D3);
//			Serial.print(", ");
//			Serial.print(Dt);
//			Serial.println(")");
//			while (Serial.read() >= 0) {}
//
//			break;
//		}
//	}
//}

void OK()
{
	Serial.println("OK");
}
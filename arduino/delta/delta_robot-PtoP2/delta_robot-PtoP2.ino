#include "DeltaRobInverseKin.h"
#include "math.h"
#include "My.h"
#include <Wire.h>


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
boolean debug = 1;

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);

double coordinate[3];			//輸入的x , y , z
int16_t D[3] = { 0,0,0 };	//抓取演算法出來的goalPos的角度
uint8_t returnData = 0;	//歸零完傳回的數值
double degreeTurn[1][3];		//與前一次相減後運轉方向與角度
double location[2][3];	//用來存之前的位置
int16_t locationNumber=0;	
int16_t ring;		//輸入位置的次數  暫時沒用到
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
	for (int i = 0; i < 3; i++)
	{
		pinMode(senser[i + 8], INPUT);
		pinMode(stepPin[i], OUTPUT);
		pinMode(dirPin[i], OUTPUT);
	}
	Serial.println(" start ");
	Serial.println();
	for (int i = 0;i < 2; i++)
	{
		for (int i2 = 0;i2< 3; i2++)
		{
			location[i][i2] = 0;			
		}
	}
	
}

void loop()
{
	Show();
	/*while (1)
	{
		Serial.println();
		Serial.println("輸入目標點的X , Y , Z");
		PtoP_input();
		ctrl_PtoP(coordinate[0], coordinate[1], coordinate[2]);
	}*/
}

//----PtoP-----輸入X Y Z的函式
void PtoP_input()
{
	Serial.println("PtoP input		Start ");
	int D = 0;
	while (1)
	{
		if (D == 0)
		{
			Serial.println("輸入X軸參數");
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
			Serial.println("輸入Y軸參數");
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
			Serial.println("輸入Z軸參數");
			while (1)
			{
				if (Serial.available())
				{
					coordinate[2] = 0-Serial.parseInt();
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
			Serial.println("PtoP input		END ");
			break;
		}

	}
}
//----PtoP-----end----------

//-------ctrl_PtoP---------
void ctrl_PtoP(int16_t x0, int16_t y0, int16_t z0)	//輸入x,y,z進來
{	
	Serial.print("ctrl_PtoP  ");
	Serial.println("Start");
	//輸入 0 0 0 進入歸零狀態
	if (coordinate[0] == 0 && coordinate[1] == 0 && coordinate[2] == 0)
	{
		//DeltaTest.return1();		
		return0();
		AfterCompare();		//對前一次的位置進行覆寫

	}			
	Serial.print(x0);
	Serial.print(" , ");
	Serial.print(y0);
	Serial.print(" , ");
	Serial.print(z0);
	Serial.println();
	delta.setGoalCoordinates(x0*0.01, y0*0.01, z0*0.01, 0);
	delay(10);
	//進入位置比對
	LocationCompare();
	//PtoP_output();
	if (coordinate[0] != 0 || coordinate[1] != 0 || coordinate[2] != 0)
	{
		turn();

	}
	Serial.print("ctrl_PtoP			END		 ");
	
}
//-------ctrl_PtoP---------end--------

//-------- LocationCompare -------------
void LocationCompare()		//現在位置與前一次的比對
{
	Serial.println("LocationCompare		start	");
	for (int i = 0; i < 3; i++)
	{
		location[0][i] = delta.goalPos[i];

	}	
	for (int i = 0; i < 3; i++)
	{
		Serial.print("location[0]");
		Serial.println(location[0][i]);		
		Serial.print("location[1]");
		Serial.println(location[1][i]);
		Serial.println();

	}
	if (location[0] != location[1])
	{		
		Serial.println("location[0] != location[1]");		
		for (int i = 0; i < 3; i++)
		{
			degreeTurn[0][i] = location[0][i] - location[1][i];
			Serial.print("degreeTurn = ");
			Serial.println(degreeTurn[0][i]);

		}		
		AfterCompare();
		
	}
}
//-------- LocationCompare -------------end ------------

//------turn-----
void turn()		//馬達轉動函式
{
	Wire.beginTransmission(1);
	if (debug)
	{
		Serial.println("-->Turn -->beginTransmission to 1");
	}
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(1);
	if (debug)
	{
		Serial.println((int)degreeTurn[0][0]);
	}
	Wire.write((int)degreeTurn[0][0]);
	Wire.endTransmission();
	//-----2-----
	Wire.beginTransmission(2);
	if (debug)
	{
		Serial.println("-->Turn -->beginTransmission to 2");
	}
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(2);
	if (debug)
	{
		Serial.println((int)degreeTurn[0][1]);
	}
	Wire.write((int)degreeTurn[0][1]);
	Wire.endTransmission();
	//-----3-----.	
	Wire.beginTransmission(3);
	if (debug)
	{
		Serial.println("-->Turn -->beginTransmission to 3");
	}
	Wire.write("start");
	Wire.endTransmission();
	Wire.beginTransmission(3);
	if (debug)
	{
		Serial.println((int)degreeTurn[0][2]);
	}
	Wire.write((int)degreeTurn[0][2]);
	Wire.endTransmission();

}
//------turn-----end--------

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
void PtoP_output()
{
	Serial.println();
	Serial.println("PtoP_output");
	OK();
	LocationCompare();

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

//---------reutrnO---------
void return0()		//歸零函式
{
	Serial.println("歸零啟動");
	Serial.println((int)digitalRead(8));
	Serial.println(digitalRead(senser[1]));
	Serial.println(digitalRead(senser[2]));
	//-----1-----
	Wire.beginTransmission(1);
	if (debug)
	{
			Serial.println("beginTransmission to 1");
	}
	Wire.write("return0");
	Wire.endTransmission();
	Wire.beginTransmission(1);
	Wire.write("start");
	Wire.endTransmission();	
	//-----2-----
	Wire.beginTransmission(2);
	if (debug)
	{
			Serial.println("beginTransmission to 2");
	}
	Wire.write("return0");
	Wire.endTransmission();
	Wire.beginTransmission(2);
	Wire.write("start");
	Wire.endTransmission();
	//-----3-----.	
	Wire.beginTransmission(3);	
	if (debug)
	{
			Serial.println("beginTransmission to 3");
	}
	Wire.write("return0");
	Wire.endTransmission();
	Wire.beginTransmission(3);
	Wire.write("start");
	Wire.endTransmission();	
	/*delay(2000);
	while (1)
	{
			Wire.requestFrom(1,4);
			if (Wire.available())
			{
					returnData = Wire.read();
					if (debug)
					{
							Serial.println(returnData);
					}

			}
			if (returnData == 1)
			{

				break;
			}
	}*/
}
//---------reutrnO---------end----------

void OK()
{
	Serial.println("OK");
	Serial.println();
}

void ShowRE()
{
	Wire.beginTransmission(1);
	Wire.write("return0");
	Wire.endTransmission();
	Wire.beginTransmission(1);
	Wire.write("start");
	Wire.endTransmission();
	//-----2-----
	Wire.beginTransmission(2);
	Wire.write("return0");
	Wire.endTransmission();
	Wire.beginTransmission(2);
	Wire.write("start");
	Wire.endTransmission();
	//-----3-----.	
	Wire.beginTransmission(3);
	Wire.write("return0");
	Wire.endTransmission();
	Wire.beginTransmission(3);
	Wire.write("start");
	Wire.endTransmission();
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

#include "DeltaRobInverseKin.h"
#include "math.h"
#include "My.h"
#include <Wire.h>

#define L_UPPER 0.12 //�W�u��		���G����
#define L_LOWER 0.285 //�U�u��
#define WB 0.13  //���y���T�������ߨ��䪺����
#define WP 0.013  //�U��p���T���έ��ߨ����������
#define UP 0.026  //�U��p���T���έ��ߨ쳻�I������
#define SP 0.045  //�U�西�T���������

boolean debug = 1;

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);

double coordinate[3];			//��J��x , y , z
int16_t D[3] = { 0,0,0 };	//����t��k�X�Ӫ�goalPos������
uint8_t returnData = 0;	//�k�s���Ǧ^���ƭ�
double degreeTurn[1][3];		//�P�e�@���۴��B���V�P����
double location[2][3];	//�ΨӦs���e����m
int16_t locationNumber = 0;
int16_t ring;		//��J��m������  �ȮɨS�Ψ�
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
		Show();
	}	
	/*while (1)
	{
	Serial.println();
	Serial.println("��J�ؼ��I��X , Y , Z");
	PtoP_input();
	ctrl_PtoP(coordinate[0], coordinate[1], coordinate[2]);
	}*/
}

//----PtoP-----��JX Y Z���禡
void PtoP_input()
{
	Serial.println("PtoP input		Start ");
	int D = 0;
	while (1)
	{
		if (D == 0)
		{
			Serial.println("��JX�b�Ѽ�");
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
			Serial.println("��JY�b�Ѽ�");
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
			Serial.println("��JZ�b�Ѽ�");
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
			Serial.print("�ثeX , Y , Z �b�Ѽ� = ");
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
void ctrl_PtoP(int16_t x0, int16_t y0, int16_t z0)	//��Jx,y,z�i��
{
	Serial.print("ctrl_PtoP  ");
	Serial.println("Start");
	//��J 0 0 0 �i�J�k�s���A
	if (coordinate[0] == 0 && coordinate[1] == 0 && coordinate[2] == 0)
	{
		//DeltaTest.return1();		
		return0();
		AfterCompare();		//��e�@������m�i���мg

	}
	Serial.print(x0);
	Serial.print(" , ");
	Serial.print(y0);
	Serial.print(" , ");
	Serial.print(z0);
	Serial.println();
	delta.setGoalCoordinates(x0*0.01, y0*0.01, z0*0.01, 0);
	delay(10);
	//�i�J��m���
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
void LocationCompare()		//�{�b��m�P�e�@�������
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
void turn()		//���F��ʨ禡
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
void AfterCompare()		//��e�@������m�i���мg
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

	Serial.print("�T�����F��ʨ��� = ");
	for (int i = 0; i < 3; i++)
	{
		Serial.print(delta._theta2[i]);
		if (i < 2)
		{
			Serial.print(" , ");
		}
	}
	Serial.println();
	Serial.print("�T�����F��ʨ��� 2= ");
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
		Serial.print("�⦸��J�׼Ʈt = ");
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
void return0()		//�k�s�禡
{
	Serial.println("�k�s�Ұ�");	
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

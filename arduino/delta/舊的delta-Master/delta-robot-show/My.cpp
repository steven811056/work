#include "Arduino.h"
#include "My.h"
#include <math.h>
#include "DeltaRobInverseKin.h"

extern DeltaRobInverseKin delta;

MyDelta Mdelta;

MyDelta::MyDelta(double _Mycoordinate1, double  _Mycoordinate2, double  _Mycoordinate3)
{
	Mycoordinate[0] = _Mycoordinate1;
	Mycoordinate[1] = _Mycoordinate2;
	Mycoordinate[2] = _Mycoordinate3;

}

void MyDelta::PtoP()
{
	Serial.print("MyDelta -> ctrl_PtoP  ");
	Serial.println("MyDelta -> Start");
	//輸入 0 0 0 進入歸零狀態	
	Serial.print(Mycoordinate[0]);
	Serial.print(" , ");
	Serial.print(Mycoordinate[1]);
	Serial.print(" , ");
	Serial.print(Mycoordinate[2]);
	Serial.println();
	delta.setGoalCoordinates(Mycoordinate[0]*0.01, Mycoordinate[1]*0.01, Mycoordinate[2]*0.01, 0);
	delay(10);
	Serial.print("MyDelta  ->  coordinate   ->	\n");
	Serial.println(delta.goalPos[0]);
	Serial.println(delta.goalPos[1]);
	Serial.println(delta.goalPos[2]);
	Serial.println("MyDelta  ->  coordinate   ->  end");
	//進入位置比對
	LocationCompare();
	//PtoP_output();
	Serial.print("MyDelta -> ctrl_PtoP		END		 ");

}

void MyDelta::LocationCompare()	//現在位置與前一次的比對
{
	Serial.println("LocationCompare		start	");
	for (int i = 0; i < 3; i++)
	{
		Mylocation[0][i] = delta.goalPos[i];

	}
	for (int i = 0; i < 3; i++)
	{
		Serial.print("Mylocation[0]");
		Serial.println(Mylocation[0][i]);
		Serial.print("Mylocation[1]");
		Serial.println(Mylocation[1][i]);
		Serial.println();

	}
	if (Mylocation[0] != Mylocation[1])
	{
		Serial.println("Mylocation[0] != Mylocation[1]");
		for (int i = 0; i < 3; i++)
		{
			degreeTurn[0][i] = Mylocation[0][i] - Mylocation[1][i];
			Serial.print("degreeTurn = ");
			Serial.println(degreeTurn[0][i]);

		}
		AfterCompare();

	}
}

void MyDelta::AfterCompare()		//對前一次的位置進行覆寫
{
	for (int i = 0; i < 3; i++)
	{
		Mylocation[1][i] = Mylocation[0][i];

	}

}
//-------ctrl_PtoP---------
//void ctrl_PtoP(int16_t x0, int16_t y0, int16_t z0)	//輸入x,y,z進來
//{
//	Serial.print("ctrl_PtoP  ");
//	Serial.println("Start");
//	//輸入 0 0 0 進入歸零狀態
//	if (coordinate[0] == 0 && coordinate[1] == 0 && coordinate[2] == 0)
//	{
//		//DeltaTest.return1();		
//		return0();
//		AfterCompare();		//對前一次的位置進行覆寫
//
//	}
//	Serial.print(x0);
//	Serial.print(" , ");
//	Serial.print(y0);
//	Serial.print(" , ");
//	Serial.print(z0);
//	Serial.println();
//	delta.setGoalCoordinates(x0*0.01, y0*0.01, z0*0.01, 0);
//	delay(10);
//	//進入位置比對
//	LocationCompare();
//	//PtoP_output();
//	Serial.print("ctrl_PtoP		END		 ");
//	Serial.println();
//
//}
////-------ctrl_PtoP---------end--------
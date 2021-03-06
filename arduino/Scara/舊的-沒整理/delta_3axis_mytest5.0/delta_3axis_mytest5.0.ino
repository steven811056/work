#include "math.h"
#include <Wire.h>

#define first_arm 10
#define second_arm 10

//  slave 1 是控制AB節臂的馬達
//  slave 2 是控制BC節臂的馬達
//  slave 3 是控制Z軸的馬達
const int stepPin[3] = {2, 4, 6};
const int dirPin[3] = {3, 5, 7};

//int16_t Address[100][3];  //輸入位址的紀錄存放
//int16_t AddressGap[3];  //輸入與前一次所在的距離差
int16_t Max=0;  //運轉圈數
int32_t P[3] = {0, 0, 0};  //本次輸入的存放位置
uint16_t t;  //運轉時間
int16_t Test1=0,Test2=0,Test3=0;
char choose='!';  //選擇輸入模式
double L_AC;  //A到C點的距離
double thetalOne,thetalTwo;  //角BAC 與 直線AC與X的夾角
double thetal_A[2];  //A所要轉的方向跟角度
double thetal_B[2];  //B所要轉的方向跟角度
double turn[3];  //三軸所要運轉的角度
//char Hi;   //我所使用的歸零代號
//int16_t zero=10; //歸零用的感測器

union unionType
{
	byte a[4] = {0,0,0,0};
	int b;
};

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
	if(choose =='!')
	{
		Serial.println("選擇模式");
		choose=0;
	}
	while(1)  
	{     
		if(Serial.available())
		{ 
			choose=Serial.read();
			Serial.println(choose);
			if (choose == '0')
			{
				scara_reset();
				break;
			}
			if(choose == '1') //1號模式 輸入座標
			{
				Serial.println("座標模式");				
				ctrl_deg();
				choose = '!';
				Serial.println();
				return;
			}			
			/*if(Wire.requestFrom(1,4))
			{
				Test1=Wire.read();
				if(Test1==1)
				{
					Serial.println("AB節臂歸零完成");
				}
				if(Test1==2)
				{
					Serial.println("BC節臂歸零完成");
				}
			}  */
			if(choose == '2')  //2號模式 輸入角度
			{
				Serial.println("角度模式");	
				Serial.println("開始與slave通訊");
				connect();				
				Serial.println("結束與slave通訊");
				choose='!';
				Serial.println();
				break;
			}
		}  
	} 
}

//----------------座標輸入---------------
void ctrl_deg()
{
	Serial.println("輸入座標");
	while (1)
	{
		if (Serial.available())
		{
			P[0] = Serial.parseInt();
			P[1] = Serial.parseInt();
			P[2] = Serial.parseInt();
			t = Serial.parseInt();
			break;
		}
	}
	Serial.println("座標輸入完成--開始運算");
	L_AC = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	if (L_AC>30 || L_AC<10)
	{
		if (P[0]<5 && P[0]>0)
		{
			Serial.println("輸入錯誤，重新輸入");
			ctrl_deg();
		}
	}
	if (P[0] == 0)
	{
		scara_reset();
	}
	if (L_AC<30 && L_AC>10)
	{
		if (P[0] != 0)
		{
			Quadrant_Judge();
			delta_3axis();
			choose = '!';			
		}
	}
	Serial.println();
}
//********----------------座標輸入---------------

//----------象限判斷-----------
void Quadrant_Judge()
{      
	  if(P[1]>0)
	  {
			if(P[0]>0)
			{
				Serial.println("第一象限");
			}
			if(P[0]<0)
			{      
				Serial.println("第二象限");
			}
	  }
}
//*******----------象限判斷-----------

//----------座標角度轉換------------
void delta_3axis()
{   
	//  L_AC = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
	thetalOne = acos((L_AC / (2 * first_arm))) * 180 / PI;
	thetal_B[1] = (180 - (2 * thetalOne));
	thetalTwo = atan(P[0]/P[1])*180/PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
	thetal_A[1] = thetalOne+thetalTwo;  
	turn[0]=thetal_A[1]/1.8; //16微步是0.1125  一微步是1.8
	turn[1]=thetal_B[1]/1.8;	
	Serial.print("thetal_A轉動");
	Serial.print(thetal_A[1]);  
	Serial.println("度");
	Serial.print("thetal_A--byte->");
	Serial.println((byte)thetal_A[1]);
	Serial.print("thetal_B轉動");
	Serial.print(thetal_B[1]);
	Serial.println("度");  
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
	for(int i=0;i<2;i++)
	{
		thetalOne = 0;
		thetalTwo = 0;
		thetal_A[i]=0;
		thetal_B[i]=0;
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
//***********-----------歸零------------



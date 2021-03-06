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
int16_t P[3] = {0, 0, 0}; //本次輸入的存放位置
uint16_t t; //運轉時間
int16_t Test1=0,Test2=0,Test3=0;
double L_AC; //A到C點的距離
double thetalOne,thetalTwo; //角BAC 與 直線AC與X的夾角
double thetal_A[2]; //A所要轉的方向跟角度
double thetal_B[2]; //B所要轉的方向跟角度
double turn[3]; //三軸所要運轉的角度
//char Hi;  //我所使用的歸零代號
//int16_t zero=10; //歸零用的感測器


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);  
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
  while(1)
  {   
    if(Serial.available())
    { 
      P[0] = Serial.parseInt();
      P[1] = Serial.parseInt();
      P[2] = Serial.parseInt();
      t = Serial.parseInt();
      L_AC = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
      if(L_AC>30 || L_AC<10)
      {
        if(P[0]<5 && P[0]>0)
        {
          Serial.println("輸入錯誤，重新輸入");
          break;
        }
      }
      if(P[0]==0)
      {
        scara_reset();
        break;
      }
      Quadrant_Judge();
      ctrl_deg(P[0],P[1],P[2],t);
      delta_3axis();
      break;
    }
    if(Wire.requestFrom(1,4))
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
    }   
  }
      
}

void Quadrant_Judge()
{      
  if(P[1]>0)
  {
    if(P[0]>0)
    {
      Serial.println("第一象限");
      
//      Wire.beginTransmission(1);
//      Wire.write(0);
//      Wire.endTransmission();
    }
    if(P[0]<0)
    {      
      Serial.println("第二象限");
      
//      Wire.beginTransmission(1);
//      Wire.write(1);
//      Wire.endTransmission(); 
    }
  }
}

void ctrl_deg(int16_t T0, int16_t T1, int16_t T2, uint16_t Tt)
{         
  
}

void delta_3axis()
{   
//  L_AC = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是弧度，需要*180/PI
  thetalOne = acos((L_AC / (2 * first_arm))) * 180 / PI;  
  thetal_B[1] = (180 - (2 * thetalOne));
  thetalTwo = atan(P[0]/P[1])*180/PI;   //A要轉thetalTwo的度數 需要為A來判斷C點的所在象限
  thetal_A[1] = thetalOne+thetalTwo;
  turn[0]=thetalOne/0.1125;
  turn[1]=thetalTwo/0.1125;
//  Wire.beginTransmission(1);
//  Wire.write("turn");
//  Wire.write((byte)turn[0]);
//  Wire.write((byte)turn[1]);
//  Wire.endTransmission();  
  Serial.print("thetal_A轉動");
  Serial.print(thetal_A[1]);  
  Serial.println("度");
  Serial.print("thetal_A--byte->");
  Serial.println((byte)thetal_A[1]);
  Serial.print("thetal_B轉動");
  Serial.print(thetal_B[1]);
  Serial.println("度");  
}

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
  Wire.endTransmission();
  Serial.println("reset"); 
  Wire.beginTransmission(2);
  Wire.write(3);
  Wire.endTransmission();   
}

void DegreeTurn()
{
//  if(P[0]>0) //向右轉
//  { 
//    digitalWrite(dirPin,LOW);
//  }
//  if(P[0]<0) //向左轉
//  {
//    digitalWrite(dirPin,HIGH);
//  }
//  turn[1] = (thetal_B-thetal[1])/ 0.1125;
//  if ((turn[1] - int(turn[1])) * 10 >= 5)
//  {
//    turn[1] = turn[1] + 1;
//  }
//  
//  for(int i=0;i<int(turn[1]);i++) //b點旋轉
//  {
//    digitalWrite(stepPin[1], HIGH);
//    delayMicroseconds(800);
//    digitalWrite(stepPin[1], LOW);
//    delayMicroseconds(800);
//  }  
}



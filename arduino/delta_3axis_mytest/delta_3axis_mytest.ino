#include "math.h"
#include <Wire.h>

#define first_arm 15
#define second_arm 15

const int stepPin[3] = {2, 4, 6};
const int dirPin[3] = {3, 5, 7};

int16_t Address[500][3];  //輸入位址的紀錄存放
int16_t AddressGap[3];  //輸入與前一次所在的距離差
int16_t Max=0;  //運轉圈數
int16_t P[3] = {0, 0, 0}; //本次輸入的存放位置
uint16_t t; //運轉時間
int16_t Test[3];
double L_AC; //A到C點的距離
double thetal[2]; //角BAC 與 直線AC與X的夾角
double thetal_B[2]={0,0}; //B的角度
double turn[3]; //三軸所要運轉的角度
char Hi;  //我所使用的歸零代號
int16_t zero=10; //歸零用的感測器


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(zero,INPUT);
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
  ctrl_deg(Test[0], Test[1], Test[2], t);
  if(Serial.available())
  {
    Hi=Serial.read();
    if(Hi=='0')
    {
      scara_reset();
    }
  }
}

void ctrl_deg(int16_t T0, int16_t T1, int16_t T2, uint16_t Tt)
{
  P[0] = T0;
  P[1] = T1;
  P[2] = T2;
  t = Tt;  
  for(int i=0;i<3;i++)
  {
    Address[Max][i]=P[i];
  }  
  
  delta_3axis();
  Max++;
}

void delta_3axis()
{   
  L_AC = sqrt(pow(P[0], 2) + pow(P[1], 2));   //arduino的三角函數出來都是
                                        //弧度，需要*180/PI
  thetal[0] = acos((L_AC / (2 * first_arm))) * 180 / PI;
  thetal_B[0] = (180 - (2 * thetal[0]));
  thetal[1] = atan(P[0]/P[1])*180/PI;   //A要轉thetal[1]的度數 需要為A來判斷C點的所在象限
  if(Max>0)
  {
    L_AC = sqrt(pow(Address[Max-1][0], 2) + pow(Address[Max-1][1], 2));
    thetal[0] = acos((L_AC / (2 * first_arm))) * 180 / PI;
    thetal_B[1] = (180 - (2 * thetal[0]));
  }  
}

void scara_reset()
{
  int reset1=0;
  if(reset1==0) //旋轉到碰到極限開關為止
  {
    digitalWrite(dirPin,LOW);
    while(zero==HIGH)
    {
      digitalWrite(stepPin[1], HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin[1], LOW);
      delayMicroseconds(500);            
    }
    reset1=1;
  }
  delay(500);
  if(reset1==1) //向反方向旋轉90度
  {
    digitalWrite(dirPin,HIGH);
    for(int i=0;i<800;i++)
    {
      digitalWrite(stepPin[1], HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin[1], LOW);
      delayMicroseconds(500);
    }
  }  
}

void DegreeTurn()
{
  if(P[0]>0) //向右轉
  {    
    digitalWrite(dirPin,LOW);
  }
  if(P[0]<0) //向左轉
  {
    digitalWrite(dirPin,HIGH);
  }
  turn[1] = (thetal_B[0]-thetal[1])/ 0.1125;
  if ((turn[1] - int(turn[1])) * 10 >= 5)
  {
    turn[1] = turn[1] + 1;
  }
  
  for(int i=0;i<int(turn[1]);i++) //b點旋轉
  {
    digitalWrite(stepPin[1], HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin[1], LOW);
    delayMicroseconds(800);
  }
}

void DegreeOutput()
{
  
}


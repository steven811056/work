#include "DeltaRobInverseKin.h"
#include "math.h"

#define L_UPPER 0.100
#define L_LOWER 0.220
#define WB 0.109
#define WP 0.040
#define UP 0.080
#define SP 0.138

const int stepPin = 4;
const int dirPin = 3;

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
int16_t x; //宣告16位元的整數
int16_t y;
int16_t z;
uint8_t A=0;
uint8_t MyDirection; //1為逆時針0為順時針
double MaxSpeed;
double AddSpeed;
double Add=0;

void setup()
{
	Serial.begin(115200);
	delta.debugFlag = false;
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.print("目前初始位置:(");
  for(int i=0;i<3;i++)
  {
    Serial.print(delta.mygoalPos[i]);
    if(i<2)
      Serial.print(",");
  }
  Serial.println(")"); 
  Serial.println(); 
}


void loop()
{
     
    Serial.println("請輸入運轉位置");
		input();
//    Serial.print("請輸入運轉速度上限與運轉加速度 -->");
//    Serial.println("EX: 100,50");
//    Control();  
//    turn();
    output();
    
}

void Control()
{
  while(1)
  {
    if(Serial.available())
    {
      MaxSpeed = Serial.parseInt();
      AddSpeed = Serial.parseInt();
      Serial.print("速度上限為->");
      Serial.print(MaxSpeed);
      Serial.print(" , ");
      Serial.print("加速度為->");
      Serial.println(AddSpeed);
      break;
    }
  }
  
}

void turn()
{
  digitalWrite(dirPin,true);
  delay(50);  
  if(Add==0)
  {
    for(int i=10000;i>9200;i=i-100 ) // 開頭起步8個波 0.9度
    {  
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(i);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(i); 
    }
    Add=Add+AddSpeed;
  } 
  while(Add < MaxSpeed)
  { 
    for(int i=0;i<1000;i++)
    { 
      digitalWrite(stepPin, HIGH); 
      delayMicroseconds(pow(10,6)/(Add*80/3)); // (360/3200)/360 * (hz) *60 =MaxSpeed (rpm) 
      digitalWrite(stepPin, LOW);
      delayMicroseconds(pow(10,6)/(Add*80/3));
    }
    Add=Add+AddSpeed; 
  }
  while(Add == MaxSpeed || Add > MaxSpeed)
  {
     digitalWrite(stepPin, HIGH);
     delayMicroseconds(pow(10,6)/(MaxSpeed*80/3)); // (ms)=10^6/hz  (hz)=MaxSpeed*80/3  (360/3200)/360 * (hz) *60 =MaxSpeed (rpm)    
     digitalWrite(stepPin, LOW);
     delayMicroseconds(pow(10,6)/(MaxSpeed*80/3));
  }
}

void input()
{
  while(1)
  {    
    if (Serial.available()) 
    {      
      x = Serial.parseInt();
      y = Serial.parseInt();
      z = Serial.parseInt();
    
      Serial.print("input (");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.print(", ");
      Serial.print(z);
      Serial.println(")");
      delta.setGoalCoordinates(x *0.001, y*0.001, z*0.001, 0);
      
      break;  
    }
  }
}

void output()
{
  if(A>0)
  {
    A=A-1;       
    Serial.print("Start at :(");
    for(int i=0;i<3;i++)
    {        
      Serial.print(delta.posArr[A][i]);
      if(i<2)
        Serial.print(",");      
    }    
    Serial.println(")");
  
    for(int i=0;i<3;i++)
    {
      if(delta.posArr[A+1][i]>0 && delta.posArr[A][i]>0)
      {
        if(delta.posArr[A+1][i] > delta.posArr[A][i]) // ex:100->150
        {
          MyDirection=0;
        }
        else // ex:150->100
        {
          MyDirection=1;
        }
        delta.mygoalPos[i]=abs(delta.posArr[A+1][i])-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]<0 && delta.posArr[A][i]<0)  
      {
        if(delta.posArr[A+1][i] > delta.posArr[A][i])  // ex:200->210
        {
          MyDirection=0;
        }
        else  // ex:210->200
        {
          MyDirection=1;
        }
        delta.mygoalPos[i]=abs(delta.posArr[A+1][i])-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]<0 && delta.posArr[A][i]>0)  // ex:150->200
      {
        MyDirection=0;
        delta.mygoalPos[i]=18000-abs(delta.posArr[A+1][i])+18000-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]>0 && delta.posArr[A][i]<0)  //ex:200->150
      {
        MyDirection=1;
        delta.mygoalPos[i]=18000-abs(delta.posArr[A+1][i])+18000-abs(delta.posArr[A][i]);       
      }
      
    }   
    A++;    
  }
  else
  {
    Serial.println("first print");
  }
  
  Serial.print("End at ");
  Serial.print("output (");
  Serial.print(delta.goalPos[0]);
  Serial.print(", ");
  Serial.print(delta.goalPos[1]); //單位為0.01度
  Serial.print(", ");
  Serial.print(delta.goalPos[2]);
  Serial.println(")");
  Serial.println();
  if(A>0)
  {
    Serial.print("運轉度數差 :(");
    for(int i=0;i<3;i++)
    {
       Serial.print(delta.mygoalPos[i]);
        if(i<2)
          Serial.print(",");       
    }
    Serial.println(")");
    Serial.println();
  }
  A++;         
}


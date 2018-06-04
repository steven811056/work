#include "DeltaRobInverseKin.h"
#include "math.h"

#define L_UPPER 0.100
#define L_LOWER 0.220
#define WB 0.109
#define WP 0.040
#define UP 0.080
#define SP 0.138

const int stepPin[3] = {2,4,6};
const int dirPin[3] = {3,5,7};

DeltaRobInverseKin delta(L_UPPER, L_LOWER, WB, WP, UP, SP);
int16_t x; //宣告16位元的整數
int16_t y;
int16_t z;
int16_t D1;
int16_t D2;
int16_t D3;
int16_t t;
long MyTime=0;
long MyTime2=0;
uint8_t A=0;
uint8_t deg=0;
boolean MyDirection[3]; //1為逆時針0為順時針
double MaxSpeed;
double AddSpeed;
double Add=0;
double add[3];
int degreeTurn[3];

void setup()
{
	Serial.begin(115200);
	delta.debugFlag = false;
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.print("目前初始位置:(");
  for(int i=0;i<3;i++)
  {
    Serial.print(delta.posArr[0][0]);
    if(i<2)
      Serial.print(",");
  }
  Serial.println(")"); 
  Serial.println(); 
}


void loop()
{  
  Serial.println("輸入要轉的角度跟時間");
  input();
  
  ctrl_deg(D1,D2,D3,t);
  
}

//--------ctrl_deg(int16_t d1, int16_t d2, int16_t d3, uint16_t t)----------
void ctrl_deg(int16_t d1, int16_t d2, int16_t d3, uint16_t t)
{ 
  delta.posArr[deg][0]=d1;
  delta.posArr[deg][1]=d2;
  delta.posArr[deg][2]=d3;
  delta.goalPos[0]=d1;
  delta.goalPos[1]=d2;
  delta.goalPos[2]=d3;
  deg++;  
  DegreeOutput();
}
//----------------ctrl_deg(int16_t d1, int16_t d2, int16_t d3, uint16_t t) END---------------

//---------DegreeAddSpeed()-------------
// 最大速度為HIGH-delaymicroseconds(800)-LOW-delaymicroseconds(800) 
// 每送一個波花0.0016秒 1秒625個波 轉70.3125
void DegreeAddSpeed()
{  
  for(int i=0;i<3;i++)
  {
    add[i]=2*delta.goalPos[i]/pow(t,2); // a=a*x/t^2
    if(add[i]>70)
    {
      Serial.println("錯誤");
      ctrl_deg(D1,D2,D3,t);
    }
  }
}
//---------DegreeAddSpeed()  END-------------

//----------DegreeControl()-------------
void DegreeControl()
{
  for(int i=0;i<3;i++)
  {
    digitalWrite(dirPin[i],MyDirection[i]);
    delay(5); 
  }
  for(int i=0;i<3;i++)
  {      
    // 轉動度數<=5度 輸入時間<=1秒 直接以每1600microseconds的頻率送出波
    if(delta.goalPos[i]<=5 && t<=1)  
    {
      degreeTurn[i]=delta.goalPos[i]/0.1125; //要轉動的角度/一次轉動的角度 = 要輸出的次數
      if((float(delta.goalPos[i]/0.1125)-int(delta.goalPos[i]/0.1125))*10 >=5) // 輸出次數除完小數部分四捨五入
      {
        degreeTurn[i]=degreeTurn[i]+1;
      }     
      for(int k=0;k<degreeTurn[i];k++)
      {
        digitalWrite(stepPin[i], HIGH);
        delayMicroseconds(800);
        digitalWrite(stepPin[i], LOW);
        delayMicroseconds(800);
      }
    }
    // ----------------------------

    if(t>1)
    {
      for(int k=5000;k>=2000;k=k-600)
      {
        digitalWrite(stepPin[i], HIGH);
        delayMicroseconds(k);
        digitalWrite(stepPin[i], LOW);
        delayMicroseconds(k);
      } //  0.042  6圈 0.675
    }
       
    MyTime=micros(); 
    while(MyTime2 <= pow(10,5))
    {
      MyTime2=micros()-MyTime;         
    }
  }
  MyTime=0;
//  while(Add == MaxSpeed || Add > MaxSpeed)
//  {
//     digitalWrite(stepPin, HIGH);
//     delayMicroseconds(pow(10,6)/(MaxSpeed*80/3)); // (ms)=10^6/hz  (hz)=MaxSpeed*80/3  (360/3200)/360 * (hz) *60 =MaxSpeed (rpm)    
//     digitalWrite(stepPin, LOW);
//     delayMicroseconds(pow(10,6)/(MaxSpeed*80/3));
//  }
}
//----------DegreeControl()  END-------------

//---------------DegreeOutput()----------------
void DegreeOutput()
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
          MyDirection[i]=0;
        }
        else // ex:150->100
        {
          MyDirection[i]=1;
        }
        delta.mygoalPos[i]=abs(delta.posArr[A+1][i])-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]<0 && delta.posArr[A][i]<0)  
      {
        if(delta.posArr[A+1][i] > delta.posArr[A][i])  // ex:200->210
        {
          MyDirection[i]=0;
        }
        else  // ex:210->200
        {
          MyDirection[i]=1;
        }
        delta.mygoalPos[i]=abs(delta.posArr[A+1][i])-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]<0 && delta.posArr[A][i]>0)  // ex:150->200
      {
        MyDirection[i]=0;
        delta.mygoalPos[i]=180-abs(delta.posArr[A+1][i])+180-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]>0 && delta.posArr[A][i]<0)  //ex:200->150
      {
        MyDirection[i]=1;
        delta.mygoalPos[i]=180-abs(delta.posArr[A+1][i])+180-abs(delta.posArr[A][i]);       
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
//---------------DegerrOutput()  END------------------

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

void input()
{
  while(1)
  {    
    if (Serial.available()) 
    {      
      D1 = Serial.parseInt();
      D2 = Serial.parseInt();
      D3 = Serial.parseInt();
      t = Serial.parseInt();
      Serial.print("input (");
      Serial.print(D1);
      Serial.print(", ");
      Serial.print(D2);
      Serial.print(", ");
      Serial.print(D3);
      Serial.print(", ");
      Serial.print(t);
      Serial.println(")");  
      while(Serial.read() >= 0){}
          
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
          MyDirection[i]=0;
        }
        else // ex:150->100
        {
          MyDirection[i]=1;
        }
        delta.mygoalPos[i]=abs(delta.posArr[A+1][i])-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]<0 && delta.posArr[A][i]<0)  
      {
        if(delta.posArr[A+1][i] > delta.posArr[A][i])  // ex:200->210
        {
          MyDirection[i]=0;
        }
        else  // ex:210->200
        {
          MyDirection[i]=1;
        }
        delta.mygoalPos[i]=abs(delta.posArr[A+1][i])-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]<0 && delta.posArr[A][i]>0)  // ex:150->200
      {
        MyDirection[i]=0;
        delta.mygoalPos[i]=18000-abs(delta.posArr[A+1][i])+18000-abs(delta.posArr[A][i]);
      }
      if(delta.posArr[A+1][i]>0 && delta.posArr[A][i]<0)  //ex:200->150
      {
        MyDirection[i]=1;
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


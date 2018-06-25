#include "math.h"

const int dirPin[3] = {3,5,7};    //A4988的dirPin腳位宣告
const int stepPin[3] = {2,4,6};   //A4988的stepPin腳位宣告

char Kevin; //轉動方案變數
char Kevin2; //選擇delta或是scara的變數
int delayKevin;  
int delayKevin2 = 1500; //delay的時間
int degreeKevin = 1400;  //轉動圈數

void setup()
{
  Serial.begin(9600);
  pinMode(stepPin[0], OUTPUT);
  pinMode(dirPin[0], OUTPUT);
  pinMode(stepPin[1], OUTPUT);
  pinMode(dirPin[1], OUTPUT);
  pinMode(stepPin[2], OUTPUT);
  pinMode(dirPin[2], OUTPUT);
}

void loop()
{    
  input(); 
}

void input()
{  
//  Serial.println("選擇要使用Kevin-Scara(S)或是Kevin-Delta(D)");
//  while(1)
//  {
//    if(Serial.available())
//    {
//      Kevin2 =Serial.read();
//      if(Kevin2 == 'S')
//      {
//        Serial.println("輸入要轉動的方案");
//        while(1)
//        {
//            
//        }
//      }
//      if(Kevin2 == 'D')
//      { // Delta -- Start
        while(1)
        {
          Serial.println("輸入要轉動的方案");  
          while(1)
          {    
            if (Serial.available()) 
            { 
              Kevin=Serial.read();
//              while(Serial.read() >= 0){}
//              Serial.println("請輸入轉動角度");
//              while(1)
//              {
//                if(Serial.available())
//                {
//                  degreeKevin=Serial.parseInt();
//                  while(Serial.read() >= 0){}
//                  break;
//                }
//              }
//              Serial.println(degreeKevin);              
              if(Kevin == 'A')
              { 
                outputA();
                Kevin ='0';                    
              } 
              else if(Kevin == 'B')
              { 
                outputB();
                Kevin ='0';               
              } 
              else if(Kevin == 'C')
              { 
                outputC();  
                Kevin ='0';             
              } 
              else
              {
                Serial.println("輸入錯誤請重新輸入");
                while(Serial.read() >= 0){}
                break;
              }
              while(Serial.read() >= 0){}
              break;  
            }
          }
        }
//        
//      } // Delta --END
//    }
//  }  
}

void outputA()
{
  Serial.println("A方案");
  digitalWrite(dirPin[0],HIGH);
  digitalWrite(dirPin[1],LOW);         
  digitalWrite(dirPin[2],LOW);
  delay(10);  
  for(int i=0;i<degreeKevin;i++)
  {
    if(i == 0)
    {
      delayKevin=delayKevin2+1000;
    }       
    digitalWrite(stepPin[0],HIGH);
    if(i<(degreeKevin/2))
    {
      digitalWrite(stepPin[1],HIGH);
      digitalWrite(stepPin[2],HIGH);
    }    
    delayMicroseconds(delayKevin);
    digitalWrite(stepPin[0],LOW); 
    if(i<(degreeKevin/2))
    {
      digitalWrite(stepPin[1],LOW);
      digitalWrite(stepPin[2],LOW);
    }
    delayMicroseconds(delayKevin);
    if(delayKevin > delayKevin2)
    {
      delayKevin=delayKevin-100;    
    }
  } 
  delay(3000);
  digitalWrite(dirPin[0],LOW);
  digitalWrite(dirPin[1],HIGH);         
  digitalWrite(dirPin[2],HIGH); 
  delay(10);
  for(int i=0;i<degreeKevin;i++)
  {
    if(i == 0)
    {
      delayKevin=delayKevin2+1000;
    }
    digitalWrite(stepPin[0],HIGH);
    if(i<(degreeKevin/2))
    {
      digitalWrite(stepPin[1],HIGH);
      digitalWrite(stepPin[2],HIGH);
    }
    delayMicroseconds(delayKevin);
    digitalWrite(stepPin[0],LOW); 
    if(i<(degreeKevin/2))
    {
      digitalWrite(stepPin[1],LOW);
      digitalWrite(stepPin[2],LOW);
    }
    delayMicroseconds(delayKevin);
    if(delayKevin > delayKevin2)
    {
      delayKevin=delayKevin-5;
    }
  }
}

void outputB()
{
  Serial.println("B方案");
  digitalWrite(dirPin[0],LOW);         
  digitalWrite(dirPin[1],HIGH);         
  digitalWrite(dirPin[2],LOW); 
  delay(10);
  for(int i=0;i<140;i++)
  {
    if(i == 0)
    {
      delayKevin=delayKevin2+1000;
    }  
    digitalWrite(stepPin[1],HIGH);
    if(i<70)
    {
      digitalWrite(stepPin[0],HIGH);
      digitalWrite(stepPin[2],HIGH);
    }
    delayMicroseconds(delayKevin);
    digitalWrite(stepPin[1],LOW); 
    if(i<70)
    {
      digitalWrite(stepPin[0],LOW);
      digitalWrite(stepPin[2],LOW);
    }
    delayMicroseconds(delayKevin);
    if(delayKevin > delayKevin2)
    {
      delayKevin=delayKevin-5;
    }
  }
  delay(3000);        
  digitalWrite(dirPin[0],HIGH);         
  digitalWrite(dirPin[1],LOW);         
  digitalWrite(dirPin[2],HIGH); 
  delay(10);
  for(int i=0;i<140;i++)
  {
    if(i == 0)
    {
      delayKevin=delayKevin2+1000;
    } 
    digitalWrite(stepPin[1],HIGH);
    if(i<70)
    {
      digitalWrite(stepPin[0],HIGH);
      digitalWrite(stepPin[2],HIGH);
    }
    delayMicroseconds(delayKevin);
    digitalWrite(stepPin[1],LOW); 
    if(i<70)
    {
      digitalWrite(stepPin[0],LOW);
      digitalWrite(stepPin[2],LOW);
    }
    delayMicroseconds(delayKevin);
    if(delayKevin > delayKevin2)
    {
      delayKevin=delayKevin-5;
    }
  }
}

void outputC()
{
  Serial.println("C方案");
  digitalWrite(dirPin[0],LOW);         
  digitalWrite(dirPin[1],LOW);         
  digitalWrite(dirPin[2],HIGH); 
  delay(10);
  for(int i=0;i<140;i++)
  {
    if(i == 0)
    {
      delayKevin=delayKevin2+1000;
    } 
    digitalWrite(stepPin[2],HIGH);
    if(i<70)
    {
      digitalWrite(stepPin[0],HIGH);
      digitalWrite(stepPin[1],HIGH);
    }
    delayMicroseconds(delayKevin);
    digitalWrite(stepPin[2],LOW); 
    if(i<70)
    {
      digitalWrite(stepPin[0],LOW);
      digitalWrite(stepPin[1],LOW);
    }
    delayMicroseconds(delayKevin);
    if(delayKevin > delayKevin2)
    {
      delayKevin=delayKevin-5;
    }
  }
  delay(3000);
  digitalWrite(dirPin[0],HIGH);         
  digitalWrite(dirPin[1],HIGH);         
  digitalWrite(dirPin[2],LOW); 
  delay(10);
  for(int i=0;i<140;i++)
  {
    if(i == 0)
    {
      delayKevin=delayKevin2+1000;
    } 
    digitalWrite(stepPin[2],HIGH);
    if(i<70)
    {
      digitalWrite(stepPin[0],HIGH);
      digitalWrite(stepPin[1],HIGH);
    }
    delayMicroseconds(delayKevin);
    digitalWrite(stepPin[2],LOW); 
    if(i<70)
    {
      digitalWrite(stepPin[0],LOW);
      digitalWrite(stepPin[1],LOW);
    }
    delayMicroseconds(delayKevin);
    if(delayKevin > delayKevin2)
    {
      delayKevin=delayKevin-5;
    }
  }
}
